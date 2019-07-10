#include "StateMachine.hpp"
#include "MainMenu.hpp"
#include "ScreamJump.hpp"
#include "mazeCrawler.hpp"
#include "ShootingStars.hpp"

static States currentState = MENU;

void machineSetup() {
  MainMenu::menuSetup();
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

void setupState(States state) {
  if (state == SCREAM_JUMP)
      setupGame();
  else if (state == SHOOTING_STARS)
    shootingStarsSetup();
  else if (state == MAZE_CRAWLER)
    setupMazeCrawler();
}
