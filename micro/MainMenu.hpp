#pragma once

#include "OptionsMenu.hpp"

class MainMenu {
  static OptionsMenu _optionsMenu;
  static void pressedUp();
  static void pressedDown();
  static void pressedRight();
public:
  static void menuSetup();
  static void menuLoop();
};
