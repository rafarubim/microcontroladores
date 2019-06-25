#define COLS 20
#define ROWS 4

#include "Lcd.hpp"
#include "Animation.hpp"
#include <stdlib.h>

Lcd lcd = Lcd(COLS, ROWS);

#define PIN_BUTTON 2
#define PIN_AUTOPLAY 1
#define PIN_READWRITE 10
#define PIN_CONTRAST 12

#define SPRITE_TERRAIN_EMPTY ' '      // User the ' ' character
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7

#define STAR_HORIZONTAL_POSITION 1    // Horizontal position of hero on screen

#define TERRAIN_WIDTH 20
#define TERRAIN_EMPTY 0

#define TERRAIN_LOWER_GAP_ONE 1
#define TERRAIN_LOWER_GAP_TWO 2
#define TERRAIN_UPPER_GAP_THREE 3
#define TERRAIN_UPPER_GAP_FOUR 4

#define SPRITE_STAR star

static char terrainLowerOne[TERRAIN_WIDTH + 1];
static char terrainLowerTwo[TERRAIN_WIDTH + 1];
static char terrainUpperThree[TERRAIN_WIDTH + 1];
static char terrainUpperFour[TERRAIN_WIDTH + 1];

static bool buttonPushed = false;
static bool playing = false;

void initializeGraphics() {

  for (i = 0; i < TERRAIN_WIDTH; ++i) {
    terrainLowerOne[i] = SPRITE_TERRAIN_EMPTY;
	  terrainLowerTwo[i] = SPRITE_TERRAIN_EMPTY;
	  terrainUpperThree[i] = SPRITE_TERRAIN_EMPTY;
	  terrainUpperFour[i] = SPRITE_TERRAIN_EMPTY;
  }
}

// Slide the terrain to the left in half-character increments
//
void advanceTerrain(char* terrain, byte newTerrain) {
  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    char current = terrain[i];
    char next = (i == TERRAIN_WIDTH - 1) ? newTerrain : terrain[i + 1];
    switch (current) {
      case SPRITE_TERRAIN_EMPTY:
        terrain[i] = (next == SPRITE_TERRAIN_SOLID) ? SPRITE_TERRAIN_SOLID_RIGHT : SPRITE_TERRAIN_EMPTY;
        break;
      case SPRITE_TERRAIN_SOLID:
        terrain[i] = (next == SPRITE_TERRAIN_EMPTY) ? SPRITE_TERRAIN_SOLID_LEFT : SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_RIGHT:
        terrain[i] = SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_LEFT:
        terrain[i] = SPRITE_TERRAIN_EMPTY;
        break;
    }
  }
}

bool drawStar(byte position, char* terrainLowerOne, char* terrainLowerTwo, char* terrainUpperThree, char* terrainUpperFour, unsigned int score) {
  bool collide = false;
  char lowerOneSave = terrainLowerOne[STAR_HORIZONTAL_POSITION];
  char lowerTwoSave = terrainLowerTwo[STAR_HORIZONTAL_POSITION];
  char upperThreeSave = terrainUpperThree[STAR_HORIZONTAL_POSITION];
  char upperFourSave = terrainUpperFour[STAR_HORIZONTAL_POSITION];
  byte lowerOne, lowerTwo, upperThree, upperFour;
  //switch (position)
  //{
  //case HERO_POSITION_OFF:
  //  upperFour = upperThree = lowerTwo = lowerOne = SPRITE_TERRAIN_EMPTY;
  //  break;
    // IMPLEMENTAR ***************
  //}
  if (lowerOne != ' ') {
    terrainLowerOne[STAR_HORIZONTAL_POSITION] = lowerOne;
    collide |= (lowerOneSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (lowerTwo != ' ') {
    terrainLowerTwo[STAR_HORIZONTAL_POSITION] = lowerTwo;
    collide |= (lowerTwoSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (upperThree != ' ') {
    terrainUpperThree[STAR_HORIZONTAL_POSITION] = upperThree;
    collide = (upperThreeSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (upperFour != ' ') {
    terrainUpperFour[STAR_HORIZONTAL_POSITION] = upperFour;
    collide = (upperFourSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

  // Draw the scene
  terrainLowerOne[TERRAIN_WIDTH] = '\0';
  terrainLowerTwo[TERRAIN_WIDTH] = '\0';
  terrainUpperThree[TERRAIN_WIDTH] = '\0';
  terrainUpperFour[TERRAIN_WIDTH] = '\0';
  char temp = terrainUpperFour[COLS - digits];
  terrainUpperFour[COLS - digits] = '\0';
  lcd.setCursor(0, 0);
  lcd.write(terrainUpperFour);
  terrainUpperFour[COLS - digits] = temp;
  lcd.setCursor(0, 1);
  lcd.write(terrainUpperThree);
  lcd.setCursor(0, 2);
  lcd.write(terrainLowerTwo);
  lcd.setCursor(0, 3);
  lcd.write(terrainLowerOne);
  lcd.setCursor(COLS - digits, 0);
  lcd.print(score);

  terrainUpper[STAR_HORIZONTAL_POSITION] = upperSave;
  terrainLower[STAR_HORIZONTAL_POSITION] = lowerSave;
  return collide;
}

// Handle the button push as an interrupt
void buttonPush() {
  buttonPushed = true;
}

void setup() {
  lcd.home();
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
  attachInterrupt(0/*PIN_BUTTON*/, buttonPush, FALLING);
  Animation::opening(lcd);

}

void loop() {
  static byte newTerrainType = TERRAIN_EMPTY;
  static byte TerrainDuration = 10;

  if (playing) {
    // Shift
    advanceTerrain(terrainLowerOne, newTerrainType == TERRAIN_LOWER_GAP_ONE ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
    advanceTerrain(terrainLowerTwo, newTerrainType == TERRAIN_LOWER_GAP_TWO ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
    advanceTerrain(terrainUpperThree, newTerrainType == TERRAIN_UPPER_GAP_THREE ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
    advanceTerrain(terrainUpperFour, newTerrainType == TERRAIN_UPPER_GAP_FOUR ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);

    if (--TerrainDuration == 0) {
      if (newTerrainType == TERRAIN_EMPTY) {
      int gap = rand() % 5;
      switch(gap){
      case(1):
        newTerrainType = TERRAIN_LOWER_GAP_ONE;
        break;
      case(2):
        newTerrainType = TERRAIN_LOWER_GAP_TWO;
        break;
      case(3):
        newTerrainType = TERRAIN_UPPER_GAP_THREE;
        break;
      case(4):
        newTerrainType = TERRAIN_UPPER_GAP_FOUR;
        break;
      }
      TerrainDuration = 10;
      } else {
        newTerrainType = TERRAIN_EMPTY;
      TerrainDuration = 10
      }
      
    }
  }
  delay(85);
}
