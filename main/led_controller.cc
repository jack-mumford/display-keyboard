/**
 * @file
 *
 * @license
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "led_controller.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <driver/gpio.h>
#include <esp_idf_version.h>
#include <esp_timer.h>

#include "gpio_pins.h"

namespace {

constexpr char TAG[] = "LED";
constexpr uint64_t kLEDOffDelayUsec = 50 * 1000;
LEDController* g_led_controller;  // For testing.

}  // namespace

LEDController::LEDController(gpio_num_t activity_gpio)
    : activity_gpio_(activity_gpio),
      led_off_timer_(nullptr),
      rgbled_(GPIO_NUM_45, GPIO_NUM_40, HSPI_HOST) {
  g_led_controller = this;
}

LEDController::~LEDController() {
  esp_timer_stop(led_off_timer_);
  gpio_set_level(activity_gpio_, 0);
  g_led_controller = nullptr;
}

// static:
void IRAM_ATTR LEDController::ActivityTimerCb(void* param) {
  gpio_set_level(static_cast<LEDController*>(param)->activity_gpio_, 0);
}

// static:
LEDController* LEDController::GetForTesting() {
  return g_led_controller;
}

esp_err_t LEDController::Initialize() {
  esp_err_t err;

  err = rgbled_.Initialize();
  ESP_ERROR_CHECK_WITHOUT_ABORT(err);
  if (err == ESP_OK) {
    ESP_ERROR_CHECK_WITHOUT_ABORT(rgbled_.Set(APA102::Color(0, 255, 0)));
  }

#if (BOARD_FEATHERS2 == 1)
  const gpio_config_t config = {
      .pin_bit_mask = (1UL << activity_gpio_),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  err = gpio_config(&config);
  if (err != ESP_OK)
    return err;
#endif
  err = gpio_set_level(activity_gpio_, 0);
  if (err != ESP_OK)
    return err;
  return CreateActivityTimer();
}

esp_err_t LEDController::CreateActivityTimer() {
  const esp_timer_create_args_t timer_args = {
    .callback = ActivityTimerCb,
    .arg = this,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "ActivityTimer",
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 3, 0)
    .skip_unhandled_events = true,
#endif
  };
  return esp_timer_create(&timer_args, &led_off_timer_);
}

void LEDController::FlashActivityLED() {
  esp_timer_stop(led_off_timer_);
  gpio_set_level(activity_gpio_, 1);
  esp_timer_start_once(led_off_timer_, kLEDOffDelayUsec);
}

void LEDController::SetRGBLED(uint8_t red,
                              uint8_t green,
                              uint8_t blue,
                              uint8_t intensity) {
  // APA102 intensity is a 5-bit value (0..31) - hence the shift.
  ESP_ERROR_CHECK_WITHOUT_ABORT(
      rgbled_.Set(APA102::Color(red, green, blue, intensity >> 3)));
}
