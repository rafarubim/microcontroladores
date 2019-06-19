#include "Animation.hpp"
#include <arduino.h>
#define COLS 20
#define ROWS 4


const byte stars[] = { STAR, '*', '.'};

int randomStar() {
  int inx = floor(random(0, arraySize(stars)));
  return stars[inx];
}

void Animation::opening(Lcd lcd) {
  lcd.setCursor(0, ROWS-1);
  int j = 0;
  int dir = 1;
  const int animationDelay = 90;
  for (int i = 0; i < COLS+2; i++) {
    lcd.setCursor(i, j);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y+1);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y-1);
    lcd.stamp(randomStar());

    j += dir;
    if (j >= ROWS - 1) {
      dir *= -1;
    } else if (j <= 0) {
      dir *= -1;
    }
    delay(animationDelay);
    lcd.clearCol(i-2);
  }
  for (int i = COLS-1; i >= 0-2; i--) {
    lcd.setCursor(i, j);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y+1);
    lcd.stamp(randomStar(), lcd.cursorPos.x, lcd.cursorPos.y-1);
    lcd.stamp(randomStar());
    
    j += dir;
    if (j >= ROWS - 1) {
      dir *= -1;
    } else if (j <= 0) {
      dir *= -1;
    }
    delay(animationDelay);
    lcd.clearCol(i+2);
  }
}
