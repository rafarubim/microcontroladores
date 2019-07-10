#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include "screamJump.hpp"
#include <arduino.h>
#include "StateMachine.hpp"

#define COLS 20
#define ROWS 4

void setup ()
{
  Serial.begin (9600);
  Lcd::configLcd(COLS, ROWS);

  //Animation::opening();

  
  Graphics& graphics = Graphics::getInstance();
  graphics.setFps(20);

  machineSetup();
}
   
void loop ()  
{
  machineLoop();
}
