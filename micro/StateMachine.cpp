#include "StateMachine.hpp"
#include "MainMenu.hpp"
#include "ScreamJump.hpp"

static States currentState = MENU;

void machineSetup() {
  MainMenu::menuSetup();
  setupGame();
}

void machineLoop() {
  switch(currentState) {
    case MENU:
      MainMenu::menuLoop();
      break;
    case SCREAM_JUMP:
      gameLoop();
      break;
  }
}

void changeState(States state);
