#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define PIN_BUTTON 2
#define PIN_AUTOPLAY 1
#define PIN_READWRITE 10
#define PIN_CONTRAST 12

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.'         // Use the '.' character for the head
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '      // User the ' ' character
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7

#define HERO_HORIZONTAL_POSITION 1    // Horizontal position of hero on screen

#define TERRAIN_WIDTH 20
#define TERRAIN_EMPTY 0

#define TERRAIN_LOWER_GAP_ONE 1
#define TERRAIN_LOWER_GAP_TWO 2
#define TERRAIN_UPPER_GAP_THREE 3
#define TERRAIN_UPPER_GAP_FOUR 4

#define HERO_POSITION_OFF 0          // Hero is invisible
#define HERO_POSITION_RUN_LOWER_1 1  // Hero is running on lower row (pose 1)
#define HERO_POSITION_RUN_LOWER_2 2  //                              (pose 2)

#define HERO_POSITION_JUMP_1 3       // Starting a jump
#define HERO_POSITION_JUMP_2 4       // Half-way up
#define HERO_POSITION_JUMP_3 5       // Jump is on upper row
#define HERO_POSITION_JUMP_4 6       // Jump is on upper row
#define HERO_POSITION_JUMP_5 7       // Jump is on upper row
#define HERO_POSITION_JUMP_6 8       // Jump is on upper row
#define HERO_POSITION_JUMP_7 9       // Half-way down
#define HERO_POSITION_JUMP_8 10      // About to land

#define HERO_POSITION_RUN_UPPER_1 11 // Hero is running on upper row (pose 1)
#define HERO_POSITION_RUN_UPPER_2 12 //                              (pose 2)

static char terrainLowerOne[TERRAIN_WIDTH + 1];
static char terrainLowerTwo[TERRAIN_WIDTH + 1];
static char terrainUpperThree[TERRAIN_WIDTH + 1];
static char terrainUpperFour[TERRAIN_WIDTH + 1];

static bool buttonPushed = false;

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

bool drawHero(byte position, char* terrainLowerOne, char* terrainLowerTwo, char* terrainUpperThree, char* terrainUpperFour unsigned int score) {
  bool collide = false;
  char lowerOneSave = terrainLowerOne[HERO_HORIZONTAL_POSITION];
  char lowerTwoSave = terrainLowerTwo[HERO_HORIZONTAL_POSITION];
  char upperThreeSave = terrainUpperThree[HERO_HORIZONTAL_POSITION];
  char upperFourSave = terrainUpperFour[HERO_HORIZONTAL_POSITION];
  byte lowerOne, lowerTwo, upperThree, upperFour;
  switch (position) {
    case HERO_POSITION_OFF:
      upperFour = upperThree = lowerTwo = lowerOne = SPRITE_TERRAIN_EMPTY;
      break;
    // IMPLEMENTAR ***************
  }
  if (lowerOne != ' ') {
    terrainLowerOne[HERO_HORIZONTAL_POSITION] = lowerOne;
    collide |= (lowerOneSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (lowerTwo != ' ') {
    terrainLowerTwo[HERO_HORIZONTAL_POSITION] = lowerTwo;
    collide |= (lowerTwoSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (upperThree != ' ') {
    terrainUpperThree[HERO_HORIZONTAL_POSITION] = upperThree;
    collide = (upperThreeSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (upperFour != ' ') {
    terrainUpperFour[HERO_HORIZONTAL_POSITION] = upperFour;
    collide = (upperFourSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

  // Draw the scene
  terrainLowerOne[TERRAIN_WIDTH] = '\0';
  terrainLowerTwo[TERRAIN_WIDTH] = '\0';
  terrainUpperThree[TERRAIN_WIDTH] = '\0';
  terrainUpperFour[TERRAIN_WIDTH] = '\0';
  char temp = terrainUpperFour[20 - digits];
  terrainUpperFour[20 - digits] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(terrainUpperFour);
  terrainUpperFour[20 - digits] = temp;
  lcd.setCursor(0, 1);
  lcd.print(terrainUpperThree);
  lcd.setCursor(0, 2);
  lcd.print(terrainLowerTwo);
  lcd.setCursor(0, 3);
  lcd.print(terrainLowerOne);
  lcd.setCursor(20 - digits, 0);
  lcd.print(score);

  terrainUpper[HERO_HORIZONTAL_POSITION] = upperSave;
  terrainLower[HERO_HORIZONTAL_POSITION] = lowerSave;
  return collide;
}

// Handle the button push as an interrupt
void buttonPush() {
  buttonPushed = true;
}

void setup() {
  pinMode(PIN_READWRITE, OUTPUT);
  digitalWrite(PIN_READWRITE, LOW);
  pinMode(PIN_CONTRAST, OUTPUT);
  digitalWrite(PIN_CONTRAST, LOW);
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, HIGH);

  // Digital pin 2 maps to interrupt 0
  attachInterrupt(0/*PIN_BUTTON*/, buttonPush, FALLING);

  initializeGraphics();

  lcd.begin(20, 4);
}

void loop() {
  static byte heroPos = HERO_POSITION_RUN_LOWER_1;
  static byte newTerrainType = TERRAIN_EMPTY;
  static byte TerrainDuration = 10;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;

  if (!playing) {
    drawHero((blink) ? HERO_POSITION_OFF : heroPos, terrainUpper, terrainLower, distance >> 3);
    if (blink) {
      lcd.setCursor(0, 0);
      lcd.print("FlappyBird");
    }
    delay(250);
    blink = !blink;
    if (buttonPushed) {
      initializeGraphics();
      heroPos = HERO_POSITION_RUN_LOWER_1;
      playing = true;
      buttonPushed = false;
      distance = 0;
    }
    return;
  }

  // Shift the terrain to the left
  advanceTerrain(terrainLowerOne, newTerrainType == TERRAIN_LOWER_GAP_ONE ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
  advanceTerrain(terrainLowerTwo, newTerrainType == TERRAIN_LOWER_GAP_TWO ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
  advanceTerrain(terrainUpperThree, newTerrainType == TERRAIN_UPPER_GAP_THREE ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);
  advanceTerrain(terrainUpperFour, newTerrainType == TERRAIN_UPPER_GAP_FOUR ? SPRITE_TERRAIN_EMPTY : SPRITE_TERRAIN_SOLID);

  // Make new terrain to enter on the right
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

  delay(85);
}
