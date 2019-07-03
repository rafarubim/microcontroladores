#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include <GFButton.h>

#define WALL_AMOUNT 3

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

GFButton botao(A1);

static long level = 0 ; // Store the average value
static int isJumping = 0; // True (1) if character is jumping
static unsigned long score;

static int pinguSize = 5;
static int maxY = 34;
static int minY = 0 + pinguSize;
static int maxRow = 3;
static int gapSize = 9; 
static byte WALL = 0xff;
static bool playing = false;

struct Wall {
  double x;
  int gap;
  Wall(int x, int gap): x(x), gap(gap) {}
};

Wall walls[WALL_AMOUNT] = {Wall(5, random(1, 36)), Wall(7, random(1, 36)) , Wall(9, random(1, 36))};

void drawWall(Graphics graphics, Wall& wall) {
  for( int y = 1; y < 5; y++ )
    if ( y != gap )
      graphics.stamp(WALL, Pos(wall.x, 9*y));
}

double velocityY = 1;
double g = -1;
double positionY = 32;     // Initial position of the character

static void play() {
  if ( !playing )
    playing = true;
  
  velocityY = -3;
}

static void Update()
{
  positionY += velocityY * g;
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
    if ( positionX == walls[i].x && positionY != gap ) 
      return true;
  return false;
}

void setupGame ()
{
  randomSeed(analogRead(A0));
  botao.setPressHandler(play);
}
   
void gameLoop(Lcd* lcd, Graphics& graphics)  
{ 
  botao.process(); 
  if ( playing ) {
    Update();
    if ( checkCollision(walls) ) {
      playing = false;
    }
  }
  graphics.draw(PINGU, Pos(18, floor(positionY)));
  for (int i = 0; i < WALL_AMOUNT; i++) {
    drawWall(graphics, walls[i]);
  }
  
}
