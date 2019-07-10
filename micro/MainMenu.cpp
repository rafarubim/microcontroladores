#include "MainMenu.hpp"
#include "Lcd.hpp"
#include <GFButton.h>

OptionsMenu MainMenu::_optionsMenu = OptionsMenu();

static GFButton upBtn(A0, E_GFBUTTON_PULLUP);
static GFButton downBtn(A2, E_GFBUTTON_PULLUP);
static GFButton rightBtn(A1, E_GFBUTTON_PULLUP);

void MainMenu::pressedUp() {
  _optionsMenu.moveSelectionUp();
}

void MainMenu::pressedDown() {
  _optionsMenu.moveSelectionDown();
}

void MainMenu::pressedRight() {
  int inx = _optionsMenu.getSelectionInx();
  Serial.println("Inx: "+String(inx));
}

void MainMenu::menuSetup() {
   OptionsMenu();
  _optionsMenu.addOption("Escholhe ai rapidao");
  _optionsMenu.addOption("p/ eu ver um negocio");
  String selectionList[] = {"ScreamJump","Shooting Stars", "Maze crawler", "Mais um", "E outro"};
  _optionsMenu.createSelectionList(selectionList);
  upBtn.setReleaseHandler(pressedUp);
  downBtn.setReleaseHandler(pressedDown);
  rightBtn.setReleaseHandler(pressedRight);
}

void MainMenu::menuLoop() {
  
  _optionsMenu.drawOptions();
  upBtn.process();
  downBtn.process();
  rightBtn.process();
  /*
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
  }*/
}
