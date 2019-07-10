#include "StateMachine.hpp"
#include "MainMenu.hpp"
#include "ScreamJump.hpp"
#include "mazeCrawler.hpp"
#include "ShootingStars.hpp"

static States currentState = MENU;

void machineSetup() {
  MainMenu::menuSetup();
  setupGame();
  shootingStarsSetup();
  setupMazeCrawler();
}

void machineLoop() {
  switch(currentState) {
    case MENU:
      MainMenu::menuLoop();
      break;
    case SCREAM_JUMP:
      gameLoop();
      break;
    case MAZE_CRAWLER:
      mazeCrawlerLoop();
      break;
    case SHOOTING_STARS:
      shootingStarsLoop();
      break;
  }
}

void changeState(States state) {
  currentState = state;
}
