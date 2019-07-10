#include "MainMenu.hpp"
#include "Lcd.hpp"
#include "StateMachine.hpp"
#include <GFButton.h>

OptionsMenu MainMenu::_optionsMenu = OptionsMenu();

static GFButton upBtn(A0, E_GFBUTTON_PULLUP);
static GFButton downBtn(A2, E_GFBUTTON_PULLUP);
static GFButton rightBtn(A3, E_GFBUTTON_PULLUP);

void MainMenu::pressedUp() {
  _optionsMenu.moveSelectionUp();
}

void MainMenu::pressedDown() {
  _optionsMenu.moveSelectionDown();
}

void MainMenu::pressedRight() {
  int inx = _optionsMenu.getSelectionInx();
  // state 0 is menu, after that, as list displays
  setupState(inx+1);
  changeState(inx+1);
}

void MainMenu::menuSetup() {
   OptionsMenu();
  _optionsMenu.addOption("Selecione o jogo");
  String selectionList[] = {"ScreamJump","Shooting Stars", "Maze crawler"};
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
