#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include "screamJump.hpp"
#include "MainMenu.hpp"
#include <arduino.h>

#define COLS 20
#define ROWS 4

enum States {
  MENU,
  SCREAM_JUMP,
  SHOOTING_STARS,
  MAZE_CRAWLER
};

static States currentState = MENU;

void setup ()
{
  Serial.begin (9600);
  Lcd::configLcd(COLS, ROWS);

  //Animation::opening();

  MainMenu::menuSetup();

  setupGame();
}
   
void loop ()  
{
  switch(currentState) {
    case MENU:
      MainMenu::menuLoop();
      break;
    case SCREAM_JUMP:
      gameLoop();
      break;
  }
}
