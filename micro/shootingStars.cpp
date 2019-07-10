#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include "music.h"
#include <GFButton.h>
//#include "highscore.hpp"

#define WALL_AMOUNT 3

static Note shootingstars[] = {{Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Dsharp5, 1500 / 2, 375 / 2}, {Dsharp5, 250 / 2, 250 / 2}, {E5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 500 / 2, 0}, {Gsharp4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2},
  {Fsharp5, 1500 / 2, 375 / 2}, {Fsharp5, 250 / 2, 250 / 2}, {Gsharp5, 500 / 2, 375 / 2}, {Fsharp5, 500 / 2, 0}, {Dsharp5, 500 / 2, 0}, {B4, 250 / 2, 50 / 2}
};

static const char* Menu[] = {
  "   ______ ______  ____  ___________  _______  _____________   ___  ____",
  "  / __/ // / __ |/ __ |/_  __/  _/ |/ / ___/ / __/_  __/ _ | / _ |/ __/",
  " _| |/ _  / /_/ / /_/ / / / _/ //    / (_ / _| |  / / / __ |/ , _/| |  ",
  "/___/_//_/|____/|____/ /_/ /___/_/|_/|___/ /___/ /_/ /_/ |_/_/|_/___/  "
};

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
static const int buttonPin = A0;
static int buttonState = 0;
static unsigned long previousMillis = 0;
static byte WALL[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
//static byte WALL = 0xff;
static unsigned long score;
static unsigned long initial;
static int STARSize = 6;
static int maxY = 34;
static int minY = 0 + STARSize;
static int maxRow = 3;
static bool playing = false;
static double velocityY = 0;
static double gravity = 80;
static double positionY = 17;
static double positionX = 0;
static unsigned long time_flag = 0;
static double moment;

static bool outputTone = false;
static int currentNote = -1;
static const int toneGround = 6;
static const int tonePin = A7;

struct Wall {
  int x;
  int gap;
  Wall(int x, int gap): x(x), gap(gap) {}
};

static Wall walls[WALL_AMOUNT] = {Wall(7, random(0, 4)), Wall(12, random(0, 4)), Wall(17, random(0, 4))};


void startGame() {
  Lcd& lcd = Lcd::getInstance();
  lcd.clear();
  velocityY = 0;
  positionX = 0;
  positionY = 17;
  for ( int scroll = 0; scroll < 26; scroll++) {
    for ( int i = 0; i < 4; i++ ) {
      for ( int j = scroll; j < (20 + scroll); j++) {
        lcd.stamp(Menu[i][j + scroll], j - scroll, i);
      }
    }
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("Pressione o botao");
  lcd.setCursor(4, 2);
  lcd.print("para jogar");
  for (;;) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.println("btn");
      initial = millis();
      playing = !playing;
      return;
    }
  }
}

void drawWall(Wall& wall) {
  Graphics& graphics = Graphics::getInstance();
  //Lcd& lcd = Lcd::getInstance();
  for ( int y = 0; y < 5; y++ ) {
    if ( y != wall.gap ) {
        graphics.stamp(char(WALL), WALL, Pos(floor(wall.x), y));
      //lcd.stamp((char)0xff, floor(wall.x), y);
    }
  }
}


static void jump() {
  velocityY = -36;
}

static void Update(float moment, Graphics& graphics) {
  if ((int)positionX >= 119) {
    graphics.flushScreen();
    walls[0] = Wall(7, random(0, 4));
    walls[1] = Wall(12, random(0, 4));
    walls[2] = Wall(17, random(0, 4));
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
  static double cX = positionX + 2.5;
  static double cY = positionY - 3.5;
  static double radius = 2;

  for (int i = 0; i < WALL_AMOUNT; i++) {
    if ( ( (int)(positionX / 5) - 1 == (int)walls[i].x )) {
      if ( (int)(((positionY-STARSize/2) / 8) + 1) != ((int)walls[i].gap + 1) ){
        return true;
      }
    }
  }
  return false;
}

void shootingStarsSetup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(buttonPin, INPUT);
  pinMode(toneGround, OUTPUT);
  digitalWrite(toneGround, LOW);
  pinMode(tonePin, OUTPUT);

}

unsigned long shootingStarsLoop()
{
  Lcd& lcd = Lcd::getInstance();
  Graphics graphics = Graphics::getInstance();

  if ( playing ) {
    if ( checkCollision(walls) ) {
      lcd.clear();
      score = ((millis() - initial) / 1000);
      //        Jogador jogador = {"", (float)score};
      velocityY = 0;
      positionX = 0;
      positionY = 17;
      playing = !playing;
      //        return jogador;
    }
    moment = (millis() - time_flag) / 1000.;
    Update(moment, graphics);
    graphics.draw(STAR, Pos(floor(positionX), floor(positionY)));

    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      jump();
    }

    for (int i = 0; i < WALL_AMOUNT; i++) {
      drawWall(walls[i]);
    }
  }
  else {
    startGame();
  }
  time_flag = millis();
  previousMillis = play(shootingstars, (sizeof(shootingstars) / sizeof(Note)) - 1 , previousMillis, currentNote, outputTone, tonePin);
  graphics.processGraphics();
}
