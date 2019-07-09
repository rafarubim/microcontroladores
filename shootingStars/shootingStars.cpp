#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include <GFButton.h>

#define WALL_AMOUNT 3

static char* Menu[] = {" ____     _    _      __       __     ____     __     __ _      ___          ____     ____     __      ____     ____ ",
                       "|*___)   | |  |*|    /  *     /  *   (_ *_)   (  )   |  | |    / __)        /*___)   (_  _)   /*_ |   (  _ |   /*___)",
                       "|___ |   | )__( |   (  O )   (  O )    )( *    )(    |  * |   ( (__|        |___*|    *)(    | || |    ) * /   |___*|",
                       "(____|*  |*|  | |    *__/     *__/    (__)    (__)   *_|__|    *___/        (____/    (__)   |_/|_/   (__|_)   (____/"};
               
//Note shootingstars[] = {{Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
//                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Fsharp5, 500/2, 0},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2}};

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
bool outputTone = false;
const int tonePin = 6;
const int ground = A7;
unsigned long previousMillis = 0;
int current = -1;
static byte WALL = 0xff;
static unsigned long score;
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

//    if (((cX + radius) >= 5*(walls[i].x + 1)) && ((cX - radius) <= 5*(walls[i].x + 2)) && ((cY + radius) >= 8*(walls[i].gap + 1)) && ((cY - radius) <= 7*(walls[i].gap))) {
    if (((cX - radius) <= 5*(walls[i].x + 1)) && ((cX + radius) >= 5*(walls[i].x + 1))) {
      return true;
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
   
void gameLoopStar(Lcd* lcd)
{ 
  
  Graphics graphics = Graphics::getInstance();
  
  if ( playing ) {
//    previousMillis = play(shootingstars, (sizeof(shootingstars)/sizeof(Note)) - 1 , previousMillis, current, outputTone, tonePin);
    if ( checkCollision(walls) ) {
        lcd->clear();
        velocityY = 0;
        positionX = 0;
        positionY = 17;
        playing = !playing;
        return;
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
//    noTone(tonePin);
  }
  time_flag = millis();
  graphics.processGraphics();
}
