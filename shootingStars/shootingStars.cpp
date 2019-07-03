#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"

#define WALL_AMOUNT 3

static byte STAR[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b01010,
  0b10001,
  0b00000
};

static byte WALL = 0xff;

static long level = 0 ; // Store the average value
static unsigned long score;

static int STARSize = 5;
static int maxY = 34;
static int minY = 0 + STARSize;
static int maxRow = 3;
static bool playing = true;

struct Wall {
  int x;
  int gap;
  Wall(int x, int gap): x(x), gap(gap) {}
};

Wall walls[WALL_AMOUNT] = {Wall(7,random(1,5)), Wall(10, random(1,5)), Wall(14, random(1,5))};

void drawWall(Lcd* lcd, Wall& wall) {
  for( int y = 0; y < 5; y++ ){
    if ( y != wall.gap ){
      lcd->stamp(WALL, floor(wall.x), y);
    }
  }
}

double velocityY = 0;
double gravity = 1;
double positionY = 4;     // Initial position of the character
double positionX = 10;
static unsigned long time_flag = 0;
static double moment;
static void play() {
  if ( !playing )
    playing = true;
  
  velocityY = -16;
}

static void Update(float moment)
{
  if ((int)positionX >= 20){
    walls[0] = Wall(7,random(1,5));
    walls[1] = Wall(10,random(1,5));
    walls[2] = Wall(14,random(1,5));
    positionX = 1;
  }
  positionX += 15 * moment;
  velocityY += gravity * moment;
  positionY += velocityY * moment;
  if ( positionY > maxY - 2 ) { // Check maximum positionY
    positionY = maxY - 2;
  } else if ( positionY < minY ) {
    positionY = minY;
  }
  if ( velocityY != 1 )
    velocityY++;
}

static bool checkCollision(Wall walls[]) {
  for(int i = 0; i < WALL_AMOUNT; i++)
    if ( (int)positionX == (int)walls[i].x ) return true;
  return false;
}

void setupGameStar ()
{
  Serial.begin(9600);
  //randomSeed(analogRead(A0));
  //botao.setPressHandler(play);
}
   
void gameLoopStar(Lcd* lcd, Graphics& graphics)
{ 
  if ( playing ) {
    char input = Serial.read();
    if (input == 'j') {
      play();
    }
    graphics.draw(STAR, Pos(floor(positionX), floor(positionY)));
    moment = (millis() - time_flag)/1000.;
    Update(moment);
    time_flag = millis();
    if ( checkCollision(walls) ) {
      playing = false;
    }
    for (int i = 0; i < WALL_AMOUNT; i++) {
      drawWall(lcd, walls[i]);
    }
  }
}
