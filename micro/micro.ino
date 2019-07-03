#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include "screamJump.hpp"
#include <arduino.h>

#define COLS 20
#define ROWS 4

void setup ()
{
  Serial.begin (9600);
  Lcd::configLcd(COLS, ROWS);

  Animation::opening();

  setupGame();
}
   
void loop ()  
{
  Graphics graphics = Graphics::getInstance();
  Lcd lcd = Lcd::getInstance();
  
  gameLoop(lcd, graphics);
  
  graphics.processGraphics();
}
