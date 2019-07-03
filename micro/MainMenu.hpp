#pragma once

#include "OptionsMenu.hpp"

class MainMenu {
  static OptionsMenu _optionsMenu;
public:
  static void menuSetup();
  static void menuLoop();
};
