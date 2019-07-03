#include "MainMenu.hpp"
#include "Lcd.hpp"

OptionsMenu MainMenu::_optionsMenu = OptionsMenu();

void MainMenu::menuSetup() {
  _optionsMenu.addOption("Escholhe ai rapidao");
  _optionsMenu.addOption("p/ eu ver um negocio");
  String selectionList[] = {"ScreamJump","Shooting Stars", "Maze crawler", "Mais um", "E outro"};
  _optionsMenu.createSelectionList(selectionList);
}

void MainMenu::menuLoop() {
  
  _optionsMenu.drawOptions();
  
  if (Serial.available() > 0) {
    char m = Serial.read();
    if (m == 'u') {
      Serial.println("Up");
      _optionsMenu.moveSelectionUp();
    }
    else if (m == 'd') {
      Serial.println("Down");
      _optionsMenu.moveSelectionDown();
    }
  }
}
