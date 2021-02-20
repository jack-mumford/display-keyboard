#pragma once

class MainDisplay;

class Screen {
 public:
  Screen(MainDisplay& display) : display_(display) {}

  MainDisplay& disp() const { return display_; }
  virtual void Update() = 0;

 protected:
  MainDisplay& display_;
};
