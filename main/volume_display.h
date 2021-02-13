
#include <cstdint>
#include <memory>

#include <lvgl.h>

class VolumeDisplay {
 public:
  VolumeDisplay();

  /**
   * @brief Set the Volume.
   *
   * @param volume The volume (0..100). Values outide of range will be clamped.
   */
  void SetVolume(uint8_t volume);
  bool Initialize();
  void Update();

 private:
  void SetVolumeWidgetSize();

  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
  lv_obj_t* bar_;
  lv_obj_t* screen_;
  uint8_t volume_;
};
