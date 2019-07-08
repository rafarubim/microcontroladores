#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include "music.h"
#include <GFButton.h>

unsigned long previousMillis = 0;
int current = -1;
Note shootingstars[] = {{Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Fsharp5, 500/2, 0},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2}};
                 
bool outputTone = false;
const int tonePin = 6;
const int ground = A7;

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

GFButton botao(A8, E_GFBUTTON_PULLDOWN);

static byte WALL = 0xff;

static long level = 0 ; // Store the average value
static unsigned long score;

static int STARSize = 6;
static int maxY = 34;
static int minY = 0 + STARSize;
static int maxRow = 3;
static bool playing = false;

struct Wall {
  int x;
  int gap;
  Wall(int x, int gap): x(x), gap(gap) {}
};

Wall walls[WALL_AMOUNT] = {Wall(7,random(0,4)), Wall(12, random(0,4)), Wall(17, random(0,4))};

void drawWall(Lcd* lcd, Wall& wall) {
  for( int y = 0; y < 5; y++ ){
    if ( y != wall.gap ){
      lcd->stamp(WALL, floor(wall.x), y);
    }
  }
}

double velocityY = 0;
double gravity = 80;
double positionY = 17;
double positionX = 0;
static unsigned long time_flag = 0;
static double moment;

static void start() {
  if ( !playing )
    playing = true;
  
  velocityY = -36;
}

static void Update(float moment, Graphics& graphics) {
  if ((int)positionX >= 119){
    graphics.flushScreen();
    walls[0] = Wall(7,random(0,4));
    walls[1] = Wall(12,random(0,4));
    walls[2] = Wall(17,random(0,4));
    positionX = 0;
  }
  positionX += 20 * moment;
  velocityY += gravity * moment;
  positionY += velocityY * moment;
  if ( positionY >= maxY ) {
    positionY = maxY;
  } else if ( positionY < minY ) {
    positionY = minY;
  }
}

static bool checkCollision(Wall walls[]) {
  for(int i = 0; i < WALL_AMOUNT; i++)
    if ( ( (int)(positionX / 5) - 1 == (int)walls[i].x )) {
      Serial.print("posY: ");
      Serial.println((int)positionY/8);
      Serial.print("walls: ");
      Serial.println((int)walls[i].gap + 1);
      if ( (int)(positionY / 8) != ((int)walls[i].gap + 1) ){
        return true;
      }
    }
  return false;
}

void setupGameStar ()
{
  Serial.begin(9600);
  botao.setPressHandler(start);
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);
  pinMode(tonePin, OUTPUT);
}
   
void gameLoopStar(Lcd* lcd)
{ 
  char input = Serial.read();
  if (input == 'a')
    start();
  else if (input == 'b'){
    playing = !playing;
  }
  Graphics graphics = Graphics::getInstance();
  botao.process();
  if ( playing ) {
    if ( checkCollision(walls) ) {
      graphics.flushScreen();
      playing = !playing;
      velocityY = 0;
      positionX = 0;
      positionY = 17;
      return;
    }
//    previousMillis = play(shootingstars, (sizeof(shootingstars)/sizeof(Note)) - 1 , previousMillis, current, outputTone, tonePin);
    moment = (millis() - time_flag)/1000.;
    Update(moment, graphics);
    graphics.draw(STAR, Pos(floor(positionX), floor(positionY)));
    
    for (int i = 0; i < WALL_AMOUNT; i++) {
      drawWall(lcd, walls[i]);
    }
  }
//  else {
//    noTone(tonePin);
//  }
  time_flag = millis();
  graphics.processGraphics();
}
