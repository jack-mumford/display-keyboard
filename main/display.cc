#include "display.h"

#include <lvgl.h>
#include <lvgl_helpers.h>

namespace {

constexpr char TAG[] = "display";

}  // namespace

Display::Display() {
  lv_init();
  lvgl_driver_init();
}

Display::~Display() {}
