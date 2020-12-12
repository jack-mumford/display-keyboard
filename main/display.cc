#include "display.h"

#include <lvgl.h>
#include <lvgl_helpers.h>

#include "main_screen.h"

namespace {

constexpr char TAG[] = "display";

}  // namespace

Display::Display() {
  lv_init();
  lvgl_driver_init();

  screen_.reset(new MainScreen());
}

Display::~Display() = default;
