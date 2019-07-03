#include "Animation.hpp"
#include "arrayUtils.h"
#include <arduino.h>

#define COLS 20
#define ROWS 4

const byte stars[] = { '*', '.', '+', 'x'};

int randomStar() {
  int inx = floor(random(0, arraySize(stars)));
  return stars[inx];
}

void Animation::opening() {
  Lcd lcd = Lcd::getInstance();
  lcd.setCursor(0, ROWS-1);
  int i = 0;
  int dir = 1;
  const int animationDelay = 90;
  const int animationWidth = 6;
  for (int j = 0; j < COLS+animationWidth; j++) {
    lcd.setCursor(j, i);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y+1);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y-1);
    lcd.stamp(randomStar());

    i += dir;
    if (i >= ROWS - 1) {
      dir *= -1;
    } else if (i <= 0) {
      dir *= -1;
    }
    delay(animationDelay);
    lcd.clearCol(j-animationWidth);
  }
  for (int j = COLS-1; j >= 0-animationWidth; j--) {
    lcd.setCursor(j, i);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y+1);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y-1);
    lcd.stamp(randomStar());
    
    i += dir;
    if (i >= ROWS - 1) {
      dir *= -1;
    } else if (i <= 0) {
      dir *= -1;
    }
    delay(animationDelay);
    lcd.clearCol(j+animationWidth);
  }
}

void Animation::shooting() {
  Lcd lcd = Lcd::getInstance();
  byte temp[8] = {0,0,0,0,0,0,0,0};
  temp[2] = 0b10000;
  for (int col = 0; col < 20; col++) {
    for(int i = 0; i < 5; i++) {
      byte temp2[8] = {0,0,0,0,0,0,0,0};
      temp2[2] = temp[2] | temp[2] << 1 | temp[2] << 2 | temp[2] << 3;
      lcd.stampTemp(temp2, col, 2);
      temp[2] = temp[2] >> 1;
      delay(50);
      lcd.stamp(' ', col, 2);
    }
    temp[2] = 0b10000;
  }
}
