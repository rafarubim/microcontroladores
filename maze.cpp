#include <LiquidCrystal.h>

int maze[4][19] = {
  {1, 1, 2, 1, 0, 3, 2, 1, 1, 0, 2, 2, 1, 0, 2, 0, 1, 0, 2},
  {0, 0, 2, 1, 0, 1, 3, 1, 2, 2, 2, 0, 0, 2, 3, 0, 3, 2, 2},
  {3, 3, 2, 1, 2, 0, 0, 2, 0, 3, 1, 2, 2, 0, 2, 2, 0, 3, 2},
  {0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0}
};
// total width and height of maze
int len = 19, hgt = 4;
// stores dot position on maze - begins on (0, 3)
int currentX = 17, currentY = hgt-1;
// for every char that composes maze, the corresponding one with
// the dot drawn in has its index 4 ahead of the original
int addDot = 4, removeDot = -4;

// components
LiquidCrystal lcd (12, 8, 11, 5, 4, 3, 2);
int eixoX = A1; //TODO - PINOS
int eixoY = A2;

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

void stamp(char c, int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(' ');
  lcd.setCursor(x, y);
  lcd.write(c);
}

void up_cb() {
  if (currentY == 0 || maze[currentY-1][currentX]%4==1 || maze[currentY-1][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentY][currentX] + removeDot), currentX, currentY);
  maze[currentY][currentX] += removeDot;
  currentY--; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
  maze[currentY][currentX] += addDot;
}

void down_cb() {
  if (currentY == hgt-1 || maze[currentY][currentX]%4==1 || maze[currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentY][currentX] + removeDot), currentX, currentY);
  maze[currentY][currentX] += removeDot;
  currentY++; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
  maze[currentY][currentX] += addDot;
}

void right_cb() {
  if (currentX==len-1 && currentY==hgt-1) { // victory
//    // remove dot from previous position
//    stamp(byte(maze[currentY][currentX] + removeDot), currentX, currentY);
//    maze[currentY][currentX] += removeDot;
//    currentX++; // updates dot position
//    // add dot to new position
//    stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
//    maze[currentY][currentX] += addDot;

    delay(500);
    lcd.clear();
    lcd.write("YOU WIN");
  }
  if (currentX==len-1 || maze[currentY][currentX]%4==2 || maze[currentY][currentX]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentY][currentX] + removeDot), currentX, currentY);
  maze[currentY][currentX] += removeDot;
  currentX++; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
  maze[currentY][currentX] += addDot;
}

void left_cb() {
  if (currentX == 0 || maze[currentY][currentX-1]%4==2 || maze[currentY][currentX-1]%4==3) {
    return;
  }
  // remove dot from previous position
  stamp(byte(maze[currentY][currentX] + removeDot), currentX, currentY);
  maze[currentY][currentX] += removeDot;
  currentX--; // updates dot position
  // add dot to new position
  stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
  maze[currentY][currentX] += addDot;
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
  pinMode(eixoX, INPUT);
  pinMode(eixoY, INPUT);

  for (int i = 0; i < hgt; i++) {
    for (int j = 0; j < len; j++) {
      stamp(byte(maze[i][j]), j, i);
    }
  }

  // draw dot on intial position (0, 3)
  stamp(byte(maze[currentY][currentX] + addDot), currentX, currentY);
  maze[currentY][currentX] += addDot;
}

void loop() {
  // read joystick
  // if up: up_cb()
  // if left: left_cb
  // if right: right_cb
  // if down: down_cb
  int joyX = analogRead(eixoX);
  int joyY = analogRead(eixoY);

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

//  if (joyY > joyX) {
//    if (y > -x + 1023) { // control: UP
//      Serial.println("UP");
//      // up_cb();
//    }
//    else { // control: LEFT
//      Serial.println("LEFT");
//      // left_cb();
//    }
//  }
//  else {
//    if (y > -x + 1023) { // control: RIGHT
//      Serial.println("RIGHT");
//      // right_cb();
//    }
//    else { // control: DOWN
//      Serial.println("DOWN");
//      // down_cb();
//    }
//  }
}
