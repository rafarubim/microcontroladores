#include "Lcd.hpp"
#include "Animation.hpp"
#include "Graphics.hpp"
#include <arduino.h>
#include "Pos.h"
#include<GFButton.h>

#define INITIAL_PLATFORMS 3
#define PLATFORM_AMOUNT 17
#define SOUND_THRESHOLD 120

static int platformArray[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static char* gameNameAnimation[] = {
  "   ____________  _______   __  ___     ____  ____  ______ ",
  "  / __/ ___/ _ |/ __/ _ | /  |/  / __ / / / / /  |/  / _ |",
  " _| |/ /__/ , _/ _// __ |/ /|_/ / / // / /_/ / /|_/ / ___/",
  "/___/|___/_/|_/___/_/ |_/_/  /_/  |___/|____/_/  /_/_/    "
};

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

static char PLATFORM = (char)0xff;
static int playing = 0;

static int num_Measure = 10 ; // Set the number of measurements   
static int pinSignal = A4; // pin connected to pin O module sound sensor   
static GFButton downBtn(A2, E_GFBUTTON_PULLUP);
static long Sound_signal;    // Store the value read Sound Sensor   
static long sum = 0 ; // Store the total value of n measurements   
static long level = 0 ; // Store the average value
static int isJumping = 0; // True (1) if character is jumping
static unsigned long score;
static unsigned long initialTime;
static unsigned long time_flag = 0;
static double moment;

static int pinguSize = 5;
static int maxY = 27;
static int minY = 0 + pinguSize;
static int maxRow = 3;

static double velocityY = 0;
static int velocityX = 10;

static double positionY = 32;     // Initial position of the character
static double positionX = 0;

//static byte PLATFORM[8] = {
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b11111,
//  0b00000
//};

//struct Platform {
//  double x;
//  Platform(int x): x(x) {}
//  Platform(): Platform(0) {}
//};

//Platform platforms[PLATFORM_AMOUNT] = {Platform(5), Platform(7), Platform(9)};

static int checkCollision() {
  if (positionY >= maxY - 2) {
    for (int i=0; i<20; i++) {
  //    Serial.println(int(positionX/6));
      if (int(positionX/6) - INITIAL_PLATFORMS > -1) {
        Serial.println("FOR");
  //      Serial.println(int(positionX/6) - 3);
        if (!platformArray[int(positionX/6) - INITIAL_PLATFORMS]) return 1;
      }
    }
    return 0;
  }
}


void drawPlatform() {
  Lcd lcd = Lcd::getInstance();

  for (int i=0; i<PLATFORM_AMOUNT; i++) {
    if (platformArray[i]) {
      lcd.stamp(PLATFORM, i + INITIAL_PLATFORMS, 3);
    }
  }
}

static void randomizePlatforms() {
  for (int i=0; i<PLATFORM_AMOUNT; i++) {
    platformArray[i] = random(0, 2);
  }
}

static void Update(float moment)
{
  positionX += velocityX * moment;
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
  velocityY = 22;
}

static void startGame(){
  Lcd& lcd = Lcd::getInstance();
  lcd.clear();
  for ( int scroll = 0; scroll < 19; scroll++) {
    for( int i = 0; i < 4; i++ ) {
      for( int j = scroll; j < (20+scroll); j++) {
        lcd.stamp(gameNameAnimation[i][j+scroll], j-scroll, i);
      }
    }
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Pressione o botao");
  lcd.setCursor(4,2);
  lcd.print("para jogar");
}

static void btn_cb() {
  Graphics& graphics = Graphics::getInstance();
  if (!playing) {
    playing = !playing;
    initialTime = millis(); // to calculate score
    graphics.flushScreen();
    randomizePlatforms();
    drawPlatform();
    positionX = 0;
  }
}

void setupGame ()
{
  randomSeed(analogRead(0));
  downBtn.setReleaseHandler(btn_cb);
  pinMode(pinSignal, INPUT);
  randomizePlatforms();
  startGame();
}
   
void gameLoop()  
{
  downBtn.process();

  Lcd lcd = Lcd::getInstance();
  Graphics graphics = Graphics::getInstance();

  if (playing){
    if (checkCollision()) {
      delay(1000);
      lcd.clear();
      score = (millis()-initialTime)/1000;

      Jogador player;
      player.pontos = score;
      if (isPlayerRecordist(SCREAM_JUMP_TABLE, player)) {
        setupGetPlayerName(SCREAM_JUMP_TABLE, score);
        changeState(ADD_RECORD);
      }
    }
    
    for ( int i = 0 ; i <num_Measure; i ++)  
    {  
      Sound_signal = analogRead (pinSignal);
      sum = sum + Sound_signal;
    }
    level = sum / num_Measure; // Calculate the average value
    Serial.println(level);
    sum = 0 ; // Reset the sum of the measurement values
    
    if (level > SOUND_THRESHOLD) {
      OnSoundLevelHigh(level);
    }
    else {
      OnSoundLevelLow();
    }
    
    moment = (millis() - time_flag)/1000.;
    Update(moment);
    time_flag = millis();
  
    for (int i=0; i<INITIAL_PLATFORMS; i++) {
      lcd.stamp(PLATFORM, i, 3);
    }
    
    if (positionX >= 119) {
    }
   
    graphics.draw(PINGU, Pos(positionX, floor(positionY)));
    for (int i = 0; i < PLATFORM_AMOUNT; i++) {
      drawPlatform();
    }
  }

  
  graphics.processGraphics();
}
