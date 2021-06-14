#pragma once

class MainDisplay;

class Screen {
 public:
  explicit Screen(MainDisplay& display) : display_(display) {}

  MainDisplay& disp() const { return display_; }

 protected:
  MainDisplay& display_;
};
