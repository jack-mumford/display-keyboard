#include "resource_fetcher.h"

#include <cstring>
#include <memory>

#ifdef LOG_LOCAL_LEVEL
// <ssd1306.h> defines this (incorrectly), so undefine.
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <tjpgdec/src/tjpgd.h>

#include "http_client.h"
#include "main_screen.h"

namespace {

/**
 * The JPEG decoding "device" - used when decoding.
 */
struct IODEV {
  IODEV() : image_data_read_pos(0) { bzero(&lv_image, sizeof(lv_image)); }
  std::vector<uint8_t> image_data;  // Compressed image data being decompressed.
  size_t image_data_read_pos;       // Read position within |image_data|.
  lv_img_dsc_t lv_image;            // Destination decompressed image.
};

constexpr char TAG[] = "Fetcher";
constexpr EventBits_t FETCH_EVENT = BIT0;
constexpr EventBits_t FETCH_EVENT_ALL = BIT0;

bool IsJpeg(const std::string& mime_type) {
  return mime_type == "image/jpeg";
}

std::string GetFileExtension(const std::string& file_name) {
  size_t idx = file_name.rfind('.', file_name.length());
  if (idx != std::string::npos)
    return file_name.substr(idx + 1, file_name.length() - idx);

  return "";
}

std::string GetContentTypeFromUrl(const std::string& url) {
  const std::string extn = GetFileExtension(url);
  if (extn == "jpg" || extn == "JPG")
    return "image/jpeg";
  if (extn == "png" || extn == "png")
    return "image/pmg";
  return "";
}

/**
 * JPEG decompressor input callback function.
 *
 * @param jd   JPEG decompression object.
 * @param buff Pointer to the buffer where |ndata| number of bytes should be
 *             written. A null value indicates the read position shall be
 *             advanced |ndata| bytes w/o reading.
 * @param buff Number of bytes to read/skip.
 *
 * @return number of bytes read (zero on error).
 */
unsigned int jpeg_input_cb(JDEC* jd, uint8_t* buff, unsigned int ndata) {
  IODEV* dev = static_cast<IODEV*>(jd->device);

  const size_t bytes_left = dev->image_data.size() - dev->image_data_read_pos;
  if (!bytes_left)
    return 0;
  if (ndata > bytes_left)
    ndata = bytes_left;

  if (buff) {
    // Read bytes from current read position.
    std::memcpy(buff, &dev->image_data.at(dev->image_data_read_pos), ndata);
  }

  // Advance current read position.
  dev->image_data_read_pos += ndata;
  return ndata;
}

/**
 * @brief JPEG Decompresser output callback function.
 *
 * @param jd     Decompression object.
 * @param bitmap Bitmap data to be output.
 * @param rect   Rectangular region to output.
 *
 * @return 1:OK, 0:abort.
 */
int jpeg_output_cb(JDEC* jd, void* bitmap, JRECT* rect) {
  IODEV* dev = static_cast<IODEV*>(jd->device);

  constexpr uint32_t kSrcPixelSize = 3;
  const uint32_t dst_row_bytes = dev->lv_image.header.w * sizeof(lv_color_t);
  const lv_coord_t rect_num_cols = rect->right - rect->left + 1;
  lv_coord_t rect_num_rows = rect->bottom - rect->top + 1;

  const uint8_t* src = static_cast<const uint8_t*>(bitmap);
  uint8_t* dst = const_cast<uint8_t*>(dev->lv_image.data) +
                 rect->top * dst_row_bytes + rect->left * sizeof(lv_color_t);

  // Copy the pixels bounded by |rect| from |src| to |dst| bitmaps.
  do {
    lv_color16_t* d = reinterpret_cast<lv_color16_t*>(dst);
    for (lv_coord_t col = 0; col < rect_num_cols; col++) {
      *d++ = LV_COLOR_MAKE(src[0], src[1], src[2]);
      src += kSrcPixelSize;
    }
    dst += dst_row_bytes;
  } while (--rect_num_rows);

  return 1; /* Continue to decompress */
}

/**
 * @brief Scale an image (up or down).
 *
 * @param dst Destination to hold the scaled image.
 * @param src The image to be scaled.
 * @return esp_err_t
 */
esp_err_t ScaleImage(lv_img_dsc_t* dst, const lv_img_dsc_t& src) {
  configASSERT(dst->header.w <= src.header.w);
  configASSERT(dst->header.h <= src.header.h);

  dst->header.always_zero = src.header.always_zero;
  dst->header.reserved = src.header.reserved;
  dst->header.cf = src.header.cf;
  dst->data_size = dst->header.w * sizeof(lv_color_t) * dst->header.h;
  dst->data = new uint8_t[dst->data_size];
  if (!dst->data)
    return ESP_ERR_NO_MEM;

  const float x_scale = static_cast<float>(src.header.w) / dst->header.w;
  const float y_scale = static_cast<float>(src.header.h) / dst->header.h;

  lv_color_t* dst_pixel =
      const_cast<lv_color_t*>(reinterpret_cast<const lv_color_t*>(dst->data));
  const lv_color_t* src_fb = reinterpret_cast<const lv_color_t*>(src.data);
  for (lv_coord_t y = 0; y < dst->header.h; y++) {
    for (lv_coord_t x = 0; x < dst->header.w; x++) {
      // Simple nearest neighbor.
      lv_coord_t src_x = x_scale * x;
      lv_coord_t src_y = y_scale * y;
      if (src_x > src.header.w - 1)
        src_x = src.header.w - 1;
      if (src_y > src.header.h - 1)
        src_y = src.header.h - 1;
      *dst_pixel++ = src_fb[src_y * src.header.w + src_x];
    }
  }

  return ESP_OK;
}

esp_err_t TJpgDecErrToEspErr(JRESULT err) {
  switch (err) {
    case JDR_OK:
      return ESP_OK;
    case JDR_INP:
      return ESP_ERR_INVALID_STATE;
    case JDR_PAR:
      return ESP_ERR_INVALID_ARG;
    case JDR_MEM1:
      return ESP_ERR_NO_MEM;
    case JDR_MEM2:
      return ESP_ERR_INVALID_SIZE;
    case JDR_FMT1:
      return ESP_ERR_INVALID_CRC;
    case JDR_FMT2:
    case JDR_FMT3:  // JDR_FMT3 returned for progressive JPEG.
      return ESP_ERR_NOT_SUPPORTED;
    case JDR_INTR:
      return ESP_ERR_TIMEOUT;  // Not a good match, but not ESP_FAIL!
    default:
      return ESP_FAIL;
  }
}

}  // namespace

ResourceFetcher::ResourceFetcher(ResourceFetchClient* fetch_client)
    : mutex_(xSemaphoreCreateMutex()),
      event_group_(xEventGroupCreate()),
      fetch_client_(fetch_client) {
  configASSERT(fetch_client);
}

// static
ResourceFetcher* ResourceFetcher::Start(ResourceFetchClient* fetch_client) {
  static ResourceFetcher* task;

  if (task)
    return task;

  ESP_LOGD(TAG, "Starting Fetcher task");
  task = new ResourceFetcher(fetch_client);
  if (!task)
    return nullptr;
  esp_err_t err = task->Initialize();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Cannot initialize: %s", esp_err_to_name(err));
    delete task;
    task = nullptr;
    return nullptr;
  }
  return task;
}

esp_err_t ResourceFetcher::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 8 * 1024;

  if (!event_group_)
    return ESP_FAIL;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

void ResourceFetcher::QueueFetch(uint32_t request_id, std::string url) {
  if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
    return;
  requests_.emplace(request_id, url);
  xSemaphoreGive(mutex_);
  xEventGroupSetBits(event_group_, FETCH_EVENT);
}

void ResourceFetcher::DecodeAndScaleJPEG(RequestData request_data,
                                         std::vector<uint8_t> jpeg_data) {
  std::vector<uint8_t> work_pool(4096, 0x0);
  std::unique_ptr<JDEC> jd(new JDEC);
  std::unique_ptr<IODEV> iodev(new IODEV);

  if (work_pool.empty() || !jd || !iodev) {
    fetch_client_->FetchError(request_data.request_id, ESP_ERR_NO_MEM);
    return;
  }
  bzero(jd.get(), sizeof(JDEC));

  iodev->image_data = std::move(jpeg_data);

  JRESULT res = jd_prepare(jd.get(), jpeg_input_cb, work_pool.data(),
                           work_pool.size(), iodev.get());
  if (res != JDR_OK) {
    ESP_LOGE(TAG, "Failure decompressing image: %s.", request_data.url.c_str());
    fetch_client_->FetchError(request_data.request_id, TJpgDecErrToEspErr(res));
    return;
  }

  ESP_LOGV(TAG, "Got JPEG %dx%d", jd->width, jd->height);
  iodev->lv_image.header.w = jd->width;
  iodev->lv_image.header.h = jd->height;
  iodev->lv_image.header.cf = LV_IMG_CF_TRUE_COLOR;
  iodev->lv_image.data_size = jd->height * jd->width * sizeof(lv_color_t);
  iodev->lv_image.data = new uint8_t[iodev->lv_image.data_size];

  if (!iodev->lv_image.data) {
    fetch_client_->FetchError(request_data.request_id, ESP_ERR_NO_MEM);
    return;
  }

  res = jd_decomp(jd.get(), jpeg_output_cb, /*scale(1.0)=*/0);
  iodev->image_data.clear();  // No longer need compressed resource.
  if (res != JDR_OK) {
    delete[] iodev->lv_image.data;
    fetch_client_->FetchError(request_data.request_id, TJpgDecErrToEspErr(res));
    return;
  }

  lv_img_dsc_t scaled_image;
  bzero(&scaled_image, sizeof(scaled_image));
  scaled_image.header.w = kAlbumArtworkWidth;
  scaled_image.header.h = kAlbumArtworkHeight;

  esp_err_t err = ScaleImage(&scaled_image, iodev->lv_image);
  delete[] iodev->lv_image.data;  // Delete (success or not).
  if (err != ESP_OK) {
    delete[] scaled_image.data;
    fetch_client_->FetchError(request_data.request_id, err);
    return;
  }

  fetch_client_->FetchImageResult(request_data.request_id,
                                  std::move(scaled_image));
}

void ResourceFetcher::DownloadResource(RequestData request_data) {
  std::vector<uint8_t> response;
  HTTPClient https_client;
  int status_code(0);
  const std::vector<HTTPClient::HeaderValue> header_values;

  ESP_LOGD(TAG, "GETting %s", request_data.url.c_str());
  esp_err_t err = https_client.DoGET(
      request_data.url, header_values,
      [&response](const void* data, int data_len) {
        response.insert(response.end(), static_cast<const uint8_t*>(data),
                        static_cast<const uint8_t*>(data) + data_len);
        return ESP_OK;
      },
      &status_code);

  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error GETting %s: %s", request_data.url.c_str(),
             esp_err_to_name(err));
    fetch_client_->FetchError(request_data.request_id, err);
    return;
  }
  if (response.empty()) {
    fetch_client_->FetchError(request_data.request_id, ESP_ERR_INVALID_SIZE);
    return;
  }
  // TODO: Get the content-type from the HTTP response.
  std::string mime_type = GetContentTypeFromUrl(request_data.url);
  if (IsJpeg(mime_type)) {
    DecodeAndScaleJPEG(std::move(request_data), std::move(response));
    return;
  }
  ESP_LOGW(TAG, "content type \"%s\" not a JPEG", mime_type.c_str());
  fetch_client_->FetchResult(request_data.request_id, status_code,
                             std::move(response), std::move(mime_type));
}

void IRAM_ATTR ResourceFetcher::Run() {
  while (true) {
    EventBits_t bits = xEventGroupWaitBits(
        event_group_, FETCH_EVENT_ALL, /*xClearOnExit=*/pdFALSE,
        /*xWaitForAllBits=*/pdFALSE, portMAX_DELAY);
    if (bits & FETCH_EVENT) {
      if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
        return;
      configASSERT(!requests_.empty());
      RequestData request_data = requests_.front();
      requests_.pop();
      xSemaphoreGive(mutex_);
      DownloadResource(std::move(request_data));

      if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
        return;
      if (requests_.empty())
        xEventGroupClearBits(event_group_, FETCH_EVENT);
      xSemaphoreGive(mutex_);
    }
  }
}

// static
void IRAM_ATTR ResourceFetcher::TaskFunc(void* arg) {
  static_cast<ResourceFetcher*>(arg)->Run();
}
