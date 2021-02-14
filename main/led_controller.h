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

#pragma once

#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_err.h>
#include <esp_timer.h>

class LEDController {
 public:
  LEDController(gpio_num_t activity_gpio);
  ~LEDController();

  esp_err_t Initialize();
  void FlashActivityLED();

 private:
  static void IRAM_ATTR ActivityTimerCb(void*);

  esp_err_t CreateActivityTimer();

  gpio_num_t activity_gpio_;
  esp_timer_handle_t led_off_timer_;
};
