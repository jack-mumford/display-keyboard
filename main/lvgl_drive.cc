#include "lvgl_drive.h"

#include <memory>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <esp_err.h>
#include <esp_log.h>
#include <lv_misc/lv_fs.h>

namespace lvgl {

namespace {

constexpr char TAG[] = "LVGLDrive";

lv_fs_res_t ErrnoToLVGL(int errno_value) {
  switch (errno_value) {
    case 0:
      return LV_FS_RES_OK;
    case ENOENT:
      return LV_FS_RES_NOT_EX;
    default:
      break;
  }
  return LV_FS_RES_UNKNOWN;
}

bool drive_ready_cb(struct _lv_fs_drv_t*) {
  return true;
}

lv_fs_res_t file_open_cb(struct _lv_fs_drv_t* drv,
                         void* file_p,
                         const char* path,
                         lv_fs_mode_t mode) {
  std::unique_ptr<char> abs_path(new char[LV_FS_MAX_PATH_LENGTH]);
#if 1
  // Hack to restore the '/' prefix needed by SPIFFS.
  abs_path.get()[0] = '/';
  strcpy(abs_path.get() + 1, path);
#else
  // Looks like LVGL strips off the leading '/' prefix, which seems to
  // be needed by SPIFFS. This fails with errno=ENOENT(2).
  strcpy(abs_path.get(), path);
#endif

  FILE* f = nullptr;
  if (mode == LV_FS_MODE_WR)
    f = fopen(abs_path.get(), "w");
  else if (mode == LV_FS_MODE_RD)
    f = fopen(abs_path.get(), "r");
  else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    f = fopen(abs_path.get(), "rw");

  if (!f) {
    ESP_LOGW(TAG, "Can't open \"%s\", mode=0x%x, errno=%d", abs_path.get(),
             mode, errno);
    return ErrnoToLVGL(errno);
  }
  memcpy(file_p, &f, sizeof(f));  // Save the file pointer.
  ESP_LOGD(TAG, "Opened \"%s\"", abs_path.get());
  return LV_FS_RES_OK;
}

lv_fs_res_t file_close_cb(struct _lv_fs_drv_t* drv, void* file_p) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  if (fclose(f) == 0) {
    ESP_LOGD(TAG, "Closed file");
    return LV_FS_RES_OK;
  }
  return ErrnoToLVGL(errno);
}

lv_fs_res_t file_read_cb(struct _lv_fs_drv_t* drv,
                         void* file_p,
                         void* buf,
                         uint32_t btr,
                         uint32_t* br) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  *br = fread(buf, sizeof(uint8_t), btr, f);
  if (*br == btr) {
    ESP_LOGV(TAG, "Read %u bytes from file.", btr);
    return LV_FS_RES_OK;
  }
  const lv_fs_res_t err = ErrnoToLVGL(errno);
  if (err != LV_FS_RES_OK)
    ESP_LOGW(TAG, "Error %u reading %u bytes from file.", err, btr);

  return err;
}

lv_fs_res_t file_write_cb(struct _lv_fs_drv_t* drv,
                          void* file_p,
                          const void* buf,
                          uint32_t btw,
                          uint32_t* bw) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  *bw = fwrite(buf, sizeof(uint8_t), btw, f);
  if (*bw == btw)
    return LV_FS_RES_OK;
  return ErrnoToLVGL(errno);
}

lv_fs_res_t file_seek_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t pos) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  if (fseek(f, pos, SEEK_SET) == 0)
    return LV_FS_RES_OK;
  return ErrnoToLVGL(errno);
}

lv_fs_res_t file_tell_cb(struct _lv_fs_drv_t* drv,
                         void* file_p,
                         uint32_t* pos_p) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  *pos_p = ftell(f);
  if (*pos_p == -1)
    return ErrnoToLVGL(errno);
  return LV_FS_RES_OK;
}

lv_fs_res_t file_size_cb(struct _lv_fs_drv_t* drv,
                         void* file_p,
                         uint32_t* size_p) {
  FILE* f;
  memcpy(&f, file_p, sizeof(f));
  long current_pos = ftell(f);
  fseek(f, 0L, SEEK_END);
  long sz = ftell(f);
  fseek(f, current_pos, SEEK_SET);
  if (sz == -1)
    return ErrnoToLVGL(errno);
  *size_p = sz;
  return LV_FS_RES_OK;
}

lv_fs_res_t dir_open_cb(struct _lv_fs_drv_t* drv,
                        void* rddir_p,
                        const char* path) {
  ESP_LOGV(TAG, "Opening dir \"%s\"", path);
  DIR* d = opendir(path);
  if (d == nullptr)
    return ErrnoToLVGL(errno);

  memcpy(rddir_p, &d, sizeof(d));
  return LV_FS_RES_OK;
}

lv_fs_res_t dir_read_cb(struct _lv_fs_drv_t* drv, void* rddir_p, char* fn) {
  DIR* d;
  memcpy(&d, rddir_p, sizeof(d));
  struct dirent* ent = readdir(d);
  if (ent != nullptr) {
    strncpy(fn, ent->d_name, LV_FS_MAX_FN_LENGTH);
    fn[LV_FS_MAX_FN_LENGTH] = '\0';
    return LV_FS_RES_OK;
  }

  *fn = '\0';
  return ErrnoToLVGL(errno);  // Mayber not an error - could be out of entries.
}

lv_fs_res_t dir_close_cb(struct _lv_fs_drv_t* drv, void* rddir_p) {
  DIR* d;
  memcpy(&d, rddir_p, sizeof(d));
  if (closedir(d) == -1)
    return ErrnoToLVGL(errno);
  return LV_FS_RES_OK;
}

}  // namespace

// static
esp_err_t Drive::Initialize() {
  lv_fs_drv_t drv;
  lv_fs_drv_init(&drv);

  drv.letter = 'S';
  drv.file_size = sizeof(FILE*);
  drv.rddir_size = sizeof(DIR*);

  drv.ready_cb = drive_ready_cb;
  drv.open_cb = file_open_cb;
  drv.close_cb = file_close_cb;
  drv.read_cb = file_read_cb;
  drv.write_cb = file_write_cb;
  drv.seek_cb = file_seek_cb;
  drv.tell_cb = file_tell_cb;
  drv.size_cb = file_size_cb;

  drv.dir_open_cb = dir_open_cb;
  drv.dir_read_cb = dir_read_cb;
  drv.dir_close_cb = dir_close_cb;

  lv_fs_drv_register(&drv);
  return ESP_OK;
}

}  // namespace lvgl
