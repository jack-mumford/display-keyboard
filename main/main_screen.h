#pragma once

#include "screen.h"

class MainScreen : public Screen {
 public:
  MainScreen();
  ~MainScreen();

  void Update() override;
};
