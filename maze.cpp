#include <LiquidCrystal.h>
#include<GFButton.h>

// total width and height of maze
#define LEN 19
#define HGT 4
#define NUM_MAZES 4

// stores dot position on maze - begins on (0, 3)
int currentX = 0, currentY = 3;
// stores which maze player is going through (0-3)
int currentMazeIndex = 0;
// stores current maze map
int currentMaze[HGT][LEN];
// for every char that composes maze, the corresponding one with
// the dot drawn in has its index 4 ahead of the original
int addDot = 4, removeDot = -4;
// array to define maze order
// will be shuffled later to do it in random order
int mazeOrder[NUM_MAZES] = {0, 1, 2, 3};
// stores score (time of completion)^(-1) to send to highscore
float score;
// stores moment when game started to calculate playing time
static unsigned long initialTimeFlag;

// components
LiquidCrystal lcd (12, 8, 11, 5, 4, 3, 2);
// TODO (camila): botoes (pinos)

// maps
int maze0[HGT][LEN] = {
  {1, 1, 2, 1, 0, 3, 2, 1, 1, 0, 2, 2, 1, 0, 2, 0, 1, 0, 2},
  {0, 0, 2, 1, 0, 1, 3, 1, 2, 2, 2, 0, 0, 2, 3, 0, 3, 2, 2},
  {3, 3, 2, 1, 2, 0, 0, 2, 0, 3, 1, 2, 2, 0, 2, 2, 0, 3, 2},
  {0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0}
};

int maze1[HGT][LEN] = {
  {0, 2, 1, 2, 0, 0, 2, 2, 0, 1, 2, 0, 1, 3, 2, 0, 3, 0, 3},
  {3, 1, 2, 1, 3, 2, 3, 0, 0, 3, 2, 0, 3, 0, 0, 3, 0, 0, 3},
  {0, 1, 1, 2, 1, 2, 0, 3, 3, 1, 0, 0, 1, 2, 0, 0, 2, 0, 3},
  {0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 2, 0, 0}
};

int maze2[HGT][LEN] = {
  {0, 1, 3, 1, 2, 0, 3, 2, 0, 2, 0, 1, 2, 0, 1, 2, 0, 2, 2}, 
  {2, 0, 3, 2, 1, 0, 1, 2, 2, 1, 1, 3, 1, 3, 1, 2, 2, 0, 3}, 
  {2, 0, 1, 1, 1, 0, 2, 1, 3, 1, 0, 1, 0, 0, 0, 0, 3, 0, 2}, 
  {0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2, 2, 0, 2, 0}
};

int maze3[HGT][LEN] = {
  {0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0, 3, 1, 0, 1, 0, 1, 1, 3}, 
  {1, 0, 3, 3, 2, 2, 2, 2, 2, 0, 1, 1, 2, 1, 2, 0, 1, 0, 2}, 
  {0, 0, 0, 2, 0, 1, 0, 2, 2, 0, 1, 2, 2, 0, 2, 3, 2, 3, 2}, 
  {2, 2, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 0, 0}
};

int **mazeArray[NUM_MAZES] = {&maze0, &maze1, &maze2, &maze3},

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

// auxiliar functino to swap array items
void swap(int *v, int i, int j) {
	int temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

// function to randomize elements in int array
void shuffle(int *v, int n)
{
	for (int i = n - 1; i >= 1; i--)
	{
		// generate a random number j such that 0 <= j <= i
		int j = rand() % (i + 1);
		swap(v, i, j);
	}
}

// function to write on lcd on given position
void stamp(char c, int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(c);
}

void up_cb() {
  if (currentY == 0 || currentMaze[currentY-1][currentX]%4==1 || currentMaze[currentY-1][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(currentMaze[currentY][currentX] + removeDot), currentX, currentY);
  currentMaze[currentY][currentX] += removeDot;
  currentY--; // updates dot position
  // add dot to new position
  stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
  currentMaze[currentY][currentX] += addDot;
}

void down_cb() {
  if (currentY == HGT-1 || currentMaze[currentY][currentX]%4==1 || currentMaze[currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(currentMaze[currentY][currentX] + removeDot), currentX, currentY);
  currentMaze[currentY][currentX] += removeDot;
  currentY++; // updates dot position
  // add dot to new position
  stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
  currentMaze[currentY][currentX] += addDot;
}

void right_cb() {
  if (currentX==LEN-1 && currentY==HGT-1) { // victory

    if (currentMazeIndex == NUM_MAZES - 1) { // was last maze
      // remove dot from previous position
      stamp(byte(currentMaze[currentY][currentX] + removeDot), currentX, currentY);
      currentMaze[currentY][currentX] += removeDot;
      currentX++; // updates dot position
      // add dot to new position
      stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
      currentMaze[currentY][currentX] += addDot;

      score = 1000.0/(millis()-initialTimeFlag);
      Serial.print("Score: ");
      Serial.println(score);

      delay(500); // quick delay for player to see they've completed the maze
      // TODO (camila): highscore window
    }

    else { // go to next maze
      currentMaze = mazeArray[mazeOrder[++currentMazeIndex]];
      currentX = 0; currentY = 3; // resets dot position to beggining of maze
      draw_maze();
    }
  }
  if (currentX==LEN-1 || currentMaze[currentY][currentX]%4==2 || currentMaze[currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(currentMaze[currentY][currentX] + removeDot), currentX, currentY);
  currentMaze[currentY][currentX] += removeDot;
  currentX++; // updates dot position
  // add dot to new position
  stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
  currentMaze[currentY][currentX] += addDot;
}

void left_cb() {
  if (currentX == 0 || currentMaze[currentY][currentX-1]%4==2 || currentMaze[currentY][currentX-1]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(currentMaze[currentY][currentX] + removeDot), currentX, currentY);
  currentMaze[currentY][currentX] += removeDot;
  currentX--; // updates dot position
  // add dot to new position
  stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
  currentMaze[currentY][currentX] += addDot;
}

void draw_maze() {
  for (int i = 0; i < HGT; i++) {
    for (int j = 0; j < LEN; j++) {
      stamp(byte(currentMaze[i][j]), j, i);
    }
  }

  // draw dot on intial position (0, 3)
  stamp(byte(currentMaze[currentY][currentX] + addDot), currentX, currentY);
  currentMaze[currentY][currentX] += addDot;
}

void setup() {
  Serial.begin(9600);

  lcd.createChar(0, BLANK);
  lcd.createChar(1, BOTTOM);
  lcd.createChar(2, RIGHT);
  lcd.createChar(3, BOTTOMRIGHT);
  lcd.createChar(4, DOTBLANK);
  lcd.createChar(5, DOTBOTTOM);
  lcd.createChar(6, DOTRIGHT);
  lcd.createChar(7, DOTBOTTOMRIGHT);

  lcd.begin(20, 4);

  shuffle(mazeOrder, NUM_MAZES);
  currentMaze = mazeArray[mazeOrder[currentMazeIndex]];

  draw_maze();

  initialTimeFlag = millis();
}

void loop() {
  // TODO (camila): botao process

  char input = Serial.read();
  if (input == 'U') {
    up_cb();
  }
  else if (input == 'D') {
    down_cb();
  }
  else if (input == 'R') {
    right_cb();
  }
  else if (input == 'L') {
    left_cb();
  }
}