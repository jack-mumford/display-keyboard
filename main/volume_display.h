
#include <memory>

#include <i2clib/master.h>
#include <lvgl.h>

class VolumeDisplay {
 public:
  VolumeDisplay(i2c::Master master);

  bool Initialize();

 private:
  i2c::Master i2c_master_;
  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
};