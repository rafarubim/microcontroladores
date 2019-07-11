#include "mazeCrawler.hpp"
#include "music.h"
#include "Lcd.hpp"
#include<GFButton.h>
#include "StateMachine.hpp"
#include "Highscore.hpp"

#define null 0

// number of mazes player has to go through
#define NUM_MAZES 4
// total width and height of maze
#define LEN 19
#define HGT 4

//LiquidCrystal lcd(12, 8, 11, 5, 4, 3, 2);
static int firstLoop = 1;
static char* gameNameAnimation[] = {
  "   __  ______ ____  ____  ________  ___ _      ____   _______ ",
  "  /  |/  / _ /_  / / __/ / ___/ _ |/ _ | | /| / / /  / __/ _ |",
  " / /|_/ / __ |/ /_/ _/  / /__/ , _/ __ | |/ |/ / /__/ _// , _/",
  "/_/  /_/_/ |_/___/___/  |___/_/|_/_/ |_|__/|__/____/___/_/|_| "
};

static int playing = 0; // flag to tell if is on game loop

// stores dot position on maze - begins on (0, 3)
static int currentX = 0, currentY = 3;
// stores which maze player is going through (0-3)
static int currentMazeIndex = 0;
// stores index to current maze map (maze[currentMaze])
static int currentMaze;
// for every char that composes maze, the corresponding one with
// the dot drawn in has its index 4 ahead of the original
static int addDot = 4, removeDot = -4;
// array to define maze order
// will be shuffled later to do it in random order
static int mazeOrder[NUM_MAZES] = {0, 1, 2, 3};
// stores score (time of completion)^(-1) to send to highscore
static float score;
// stores moment when game started to calculate playing time
static unsigned long initialTimeFlag;

// music variables
static bool outputTone = false;
static unsigned long previousMillis = millis();
static int currentNote = -1;

// components
static GFButton upBtn(A0, E_GFBUTTON_PULLUP);
static GFButton leftBtn(A1, E_GFBUTTON_PULLUP);
static GFButton downBtn(A2, E_GFBUTTON_PULLUP);
static GFButton rightBtn(A3, E_GFBUTTON_PULLUP);
static const int toneGround = 6; //TODO (camila)
static const int tonePin = A7;

// music
static Note labyrinth[] = {
  {F2, 384, 0},{G2, 768, 1920},
  {F2, 384, 0},{G2, 768, 768},
  {Gsharp3, 768, 0},{F3, 384, 0},{C3, 384, 0},{D3, 384, 0},{F3, 1152, 384},
  {C3, 384, 0},{F3, 384, 0}, {C4, 384, 0}, {Gsharp3, 768, 0}, {G3, 768, 1152},
  {F3, 384, 0}, {G3, 768, 1920}
};

// maps
static int maze[NUM_MAZES][HGT][LEN] = {
  {
    {1, 1, 2, 1, 0, 3, 2, 1, 1, 0, 2, 2, 1, 0, 2, 0, 1, 0, 2},
    {0, 0, 2, 1, 0, 1, 3, 1, 2, 2, 2, 0, 0, 2, 3, 0, 3, 2, 2},
    {3, 3, 2, 1, 2, 0, 0, 2, 0, 3, 1, 2, 2, 0, 2, 2, 0, 3, 2},
    {0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0}
  },
  {
    {0, 2, 1, 2, 0, 0, 2, 2, 0, 1, 2, 0, 1, 3, 2, 0, 3, 0, 3},
    {3, 1, 2, 1, 3, 2, 3, 0, 0, 3, 2, 0, 3, 0, 0, 3, 0, 0, 3},
    {0, 1, 1, 2, 1, 2, 0, 3, 3, 1, 0, 0, 1, 2, 0, 0, 2, 0, 3},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 2, 0, 0}
  },
  {
    {0, 1, 3, 1, 2, 0, 3, 2, 0, 2, 0, 1, 2, 0, 1, 2, 0, 2, 2}, 
    {2, 0, 3, 2, 1, 0, 1, 2, 2, 1, 1, 3, 1, 3, 1, 2, 2, 0, 3}, 
    {2, 0, 1, 1, 1, 0, 2, 1, 3, 1, 0, 1, 0, 0, 0, 0, 3, 0, 2}, 
    {0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2, 2, 0, 2, 0}
  },
  {
    {0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0, 3, 1, 0, 1, 0, 1, 1, 3}, 
    {1, 0, 3, 3, 2, 2, 2, 2, 2, 0, 1, 1, 2, 1, 2, 0, 1, 0, 2}, 
    {0, 0, 0, 2, 0, 1, 0, 2, 2, 0, 1, 2, 2, 0, 2, 3, 2, 3, 2}, 
    {2, 2, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 0, 0}
  }
};

static byte BLANK[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

static byte DOTBLANK[8] = {
  B00000,
  B00000,
  B00000,
  B01100,
  B01100,
  B00000,
  B00000,
  B00000
};

static byte DOTBOTTOM[8] = {
  B00000,
  B00000,
  B00000,
  B01100,
  B01100,
  B00000,
  B00000,
  B11111
};

static byte DOTBOTTOMRIGHT[8] = {
  B00001,
  B00001,
  B00001,
  B01101,
  B01101,
  B00001,
  B00001,
  B11111
};

static byte DOTRIGHT[8] = {
  B00001,
  B00001,
  B00001,
  B01101,
  B01101,
  B00001,
  B00001,
  B00001
};

static byte BOTTOM[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

static byte BOTTOMRIGHT[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111
};

static byte RIGHT[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};

static void stamp(char c, int x, int y) {
  Lcd& lcd = Lcd::getInstance();
  lcd.setCursor(x, y);
  lcd.write(c);
}

// auxiliar function to swap array items
static void swap(int *v, int i, int j) {
  int temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

// function to randomize elements in int array
static void shuffle(int *v, int n)
{
  for (int i = n - 1; i >= 1; i--)
  {
    // generate a random number j such that 0 <= j <= i
    int j = random(i);
    swap(v, i, j);
  }
}

static void draw_maze() {
  for (int i = 0; i < HGT; i++) {
    for (int j = 0; j < LEN; j++) {
      stamp(byte(maze[currentMaze][i][j]), j, i);
    }
  }

  // draw dot on intial position (0, 3)
  stamp(byte(maze[currentMaze][currentY][currentX] + addDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += addDot;
}

static void up_cb() {
  Serial.println("up");
  if (!playing) return;
  if (currentY == 0 || maze[currentMaze][currentY-1][currentX]%4==1 || maze[currentMaze][currentY-1][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentMaze][currentY][currentX] + removeDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += removeDot;
  currentY--; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentMaze][currentY][currentX] + addDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += addDot;
}

static void down_cb() {
  if (!playing) { // start game
    playing = !playing;
    draw_maze();
    initialTimeFlag = millis(); // start game timer
    return;
  }
  
  if (currentY == HGT-1 || maze[currentMaze][currentY][currentX]%4==1 || maze[currentMaze][currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentMaze][currentY][currentX] + removeDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += removeDot;
  currentY++; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentMaze][currentY][currentX] + addDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += addDot;
}

static void right_cb() {
  Lcd& lcd = Lcd::getInstance();
  Serial.println("right");
  if (!playing) return;
  if (currentX==LEN-1 && currentY==HGT-1) { // victory
    if (currentMazeIndex == NUM_MAZES - 1) { // was last maze
      // remove dot from previous position
      stamp(byte(0), currentX, currentY);
      // add dot to new position
      stamp(byte(4), LEN, HGT-1);

      score = (millis()-initialTimeFlag)/1000;

      delay(1000); // quick delay for player to see they've completed the maze
      lcd.clear();

      Jogador player;
      player.pontos = score;
      if (isPlayerRecordist(MAZE_CRAWLER_TABLE, player)) {
        setupGetPlayerName(MAZE_CRAWLER_TABLE, score);
        changeState(ADD_RECORD);
      }
      
      // resetting buttons
      upBtn.setReleaseHandler(null);
      downBtn.setReleaseHandler(null);
      rightBtn.setReleaseHandler(null);
      leftBtn.setReleaseHandler(null);
      return;
    }

    else { // go to next maze
     currentMaze = mazeOrder[++currentMazeIndex];
      // remove dot from previous position
      stamp(byte(0), currentX, currentY);
      // resets dot position to beggining of next maze
      currentX = 0; currentY = 3;
      draw_maze();
      return;
    }
  }
  if (currentX==LEN-1 || maze[currentMaze][currentY][currentX]%4==2 || maze[currentMaze][currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentMaze][currentY][currentX] + removeDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += removeDot;
  currentX++; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentMaze][currentY][currentX] + addDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += addDot;
}

static void left_cb() {
  Serial.println("left");
  if (!playing) return;
  if (currentX == 0 || maze[currentMaze][currentY][currentX-1]%4==2 || maze[currentMaze][currentY][currentX-1]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentMaze][currentY][currentX] + removeDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += removeDot;
  currentX--; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentMaze][currentY][currentX] + addDot), currentX, currentY);
  maze[currentMaze][currentY][currentX] += addDot;
}

static void startGame(){
  Lcd& lcd = Lcd::getInstance();
  lcd.clear();
  for ( int scroll = 0; scroll < 22; scroll++) {
    for( int i = 0; i < 4; i++ ) {
      for( int j = scroll; j < (20+scroll); j++) {
        stamp(gameNameAnimation[i][j+scroll], j-scroll, i);
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

void setupMazeCrawler() {
  Serial.begin(9600);
  Lcd& lcd = Lcd::getInstance();
  randomSeed(analogRead(0));

  pinMode(toneGround, OUTPUT);
  digitalWrite(toneGround, LOW);
  pinMode(tonePin, OUTPUT);
//
  lcd.createChar(0, BLANK);
  lcd.createChar(1, BOTTOM);
  lcd.createChar(2, RIGHT);
  lcd.createChar(3, BOTTOMRIGHT);
  lcd.createChar(4, DOTBLANK);
  lcd.createChar(5, DOTBOTTOM);
  lcd.createChar(6, DOTRIGHT);
  lcd.createChar(7, DOTBOTTOMRIGHT);
  upBtn.setReleaseHandler(up_cb);
  downBtn.setReleaseHandler(down_cb);
  rightBtn.setReleaseHandler(right_cb);
  leftBtn.setReleaseHandler(left_cb);

  shuffle(mazeOrder, NUM_MAZES);
  currentMaze = mazeOrder[currentMazeIndex];

  startGame();
}

void mazeCrawlerLoop() {
  
  upBtn.process(); downBtn.process(); rightBtn.process(); leftBtn.process();

  previousMillis = play(labyrinth, (sizeof(labyrinth)/sizeof(Note)) - 1 , previousMillis, currentNote, outputTone, tonePin);
}
