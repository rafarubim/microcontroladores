#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"

#define PLATFORM_AMOUNT 3

static byte PINGU[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b01110,
  0b01100,
  0b01100,
  0b01100,
  0b01010
};

static int num_Measure = 10 ; // Set the number of measurements   
static int pinSignal = A0; // pin connected to pin O module sound sensor   
static long Sound_signal;    // Store the value read Sound Sensor   
static long sum = 0 ; // Store the total value of n measurements   
static long level = 0 ; // Store the average value
static int isJumping = 0; // True (1) if character is jumping
static unsigned long score;
static unsigned long time_flag = 0; // Resets if player on the ground
static double moment;

static int pinguSize = 5;
static int maxY = 34;
static int minY = 0 + pinguSize;

static byte PLATFORM[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};

struct Platform {
  double x;
  Platform(int x): x(x) {}
  Platform(): Platform(0) {}
};

Platform platforms[PLATFORM_AMOUNT] = {Platform(24), Platform(40), Platform(58)};

void drawPlatform(Graphics& graphics, Platform& platform) {
  graphics.draw(PLATFORM, Pos(platform.x, maxY));
  graphics.draw(PLATFORM, Pos(platform.x + 5, maxY));
}

double velocityY = 0;
double positionY = 32;     // Initial position of the character

static void Update(float moment)
{
  positionY += velocityY * moment;
  if (positionY > maxY - 2) { // Check maximun positionY
    positionY = maxY - 2;
  } else if (positionY < minY) {
    positionY = minY;
  }
}

static void OnSoundLevelHigh(long level)
{
  velocityY = -map(level, 0, 1021, 15, 50);
}

static void OnSoundLevelLow()
{ 
  velocityY = 16;
}

void setupGame ()
{
  randomSeed(analogRead(A0));
  
  pinMode(pinSignal, INPUT);
}
   
void gameLoop(Graphics& graphics)  
{  
  // Performs 20 signal readings
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
    Sound_signal = analogRead (pinSignal);
    sum = sum + Sound_signal;
  }
  level = sum / num_Measure; // Calculate the average value
  sum = 0 ; // Reset the sum of the measurement values
  
  if (level > 45) {
    Serial.println("Level: "+String(level));
    OnSoundLevelHigh(level);
  }
  else {
    OnSoundLevelLow();
  }
  
  moment = (millis() - time_flag)/1000.;
  Update(moment);
  time_flag = millis();
 
  graphics.draw(PINGU, Pos(18, floor(positionY)));
  for (int i = 0; i < PLATFORM_AMOUNT; i++) {
    drawPlatform(graphics, platforms[i]);
  }
}
