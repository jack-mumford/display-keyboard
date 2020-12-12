#pragma once

#include <memory>

#include "screen.h"

class Display {
  public:
    Display();
    ~Display();

  private:
    std::unique_ptr<Screen> screen_;
};
