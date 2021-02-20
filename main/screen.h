#pragma once

class MainDisplay;

class Screen {
 public:
  Screen(MainDisplay& display) : display_(display) {}

  MainDisplay& disp() const { return display_; }

 protected:
  MainDisplay& display_;
};
