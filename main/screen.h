#pragma once

class Display;

class Screen {
 public:
  Screen(Display& display) : display_(display) {}

  Display& disp() const { return display_; }
  virtual void Update() = 0;

 protected:
  Display& display_;
};
