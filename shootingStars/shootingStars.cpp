#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include <GFButton.h>

#define WALL_AMOUNT 3

static const char* Menu[] = {" ____     _    _      __       __     ____     __     __ _      ___          ____     ____     __      ____     ____ ",
                             "|*___)   | |  |*|    /  *     /  *   (_ *_)   (  )   |  | |    / __)        /*___)   (_  _)   /*_ |   (  _ |   /*___)",
                             "|___ |   | )__( |   (  O )   (  O )    )( *    )(    |  * |   ( (__|        |___*|    *)(    | || |    ) * /   |___*|",
                             "(____|*  |*|  | |    *__/     *__/    (__)    (__)   *_|__|    *___/        (____/    (__)   |_/|_/   (__|_)   (____/"};

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
const int buttonPin = A1;
int buttonState = 0;
unsigned long previousMillis = 0;
static byte WALL = 0xff;
static unsigned long score;
static unsigned long initial;
static int STARSize = 6;
static int maxY = 34;
static int minY = 0 + STARSize;
static int maxRow = 3;
static bool playing = false;
double velocityY = 0;
double gravity = 80;
double positionY = 17;
double positionX = 0;
static unsigned long time_flag = 0;
static double moment;


struct Wall {
  int x;
  int gap;
  Wall(int x, int gap): x(x), gap(gap) {}
};

Wall walls[WALL_AMOUNT] = {Wall(7,random(0,4)), Wall(12, random(0,4)), Wall(17, random(0,4))};


void startGame(){
  Lcd lcd = Lcd::getInstance();
  lcd.clear();
  velocityY = 0;
  positionX = 0;
  positionY = 17;
  for ( int scroll = 0; scroll < 49; scroll++) {
    for( int i = 0; i < 4; i++ ) {
      for( int j = scroll; j < (20+scroll); j++) {
        lcd.stamp(Menu[i][j+scroll], j-scroll, i);
      }
    }
    if (scroll == 48) {
      Animation::opening();
    }
    delay(100);
  }
  lcd.setCursor(1,1);
  lcd.print("Pressione o botao");
  lcd.setCursor(4,2);
  lcd.print("para jogar");
  for(;;){
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      initial = millis();
      playing = !playing;
      return;
    }
    lcd.noDisplay();
    delay(500);
    lcd.display();
    delay(500);
  }
}

void drawWall(Lcd* lcd, Wall& wall) {
  for( int y = 0; y < 5; y++ ){
    if ( y != wall.gap ){
      lcd->stamp(WALL, floor(wall.x), y);
    }
  }
}


static void jump() {
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
  double cX = positionX + 2.5;
  double cY = positionY - 3.5;
  double radius = 2;
  
  for(int i = 0; i < WALL_AMOUNT; i++){
    if ( ( (int)(positionX / 5) - 1 == (int)walls[i].x )) {
      if ( (int)(positionY / 8) != ((int)walls[i].gap + 1) ){
        return true;
      }
    }
  }
  return false;
}

void setupGameStar ()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);
  pinMode(tonePin, OUTPUT);
  
}

unsigned long gameLoopStar(Lcd* lcd)
{ 
  
  Graphics graphics = Graphics::getInstance();
  
  if ( playing ) {
    if ( checkCollision(walls) ) {
        lcd->clear();
        score = ((millis() - initial) / 1000);
        velocityY = 0;
        positionX = 0;
        positionY = 17;
        playing = !playing;
        return score;
    }
    moment = (millis() - time_flag)/1000.;
    Update(moment, graphics);
    graphics.draw(STAR, Pos(floor(positionX), floor(positionY)));
    
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      jump();
    }
    
    for (int i = 0; i < WALL_AMOUNT; i++) {
      drawWall(lcd, walls[i]);
    }
  }
  else {
    startGame();
  }
  time_flag = millis();
  graphics.processGraphics();
}
