/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K potentiometer:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 10K poterntiometer on pin A0
*/
#define COLS 20
#define ROWS 4

#include "Lcd.hpp"

void setup() {
  Lcd lcd = Lcd(COLS, ROWS);
  lcd.setCursor(0, ROWS-1);
  int j = 0;
  int dir = -1;
  for (int i = 0; i < COLS; i++) {
    lcd.write(STAR);
    lcd.moveCursor(0, dir);
    j++;
    if (j >= ROWS - 1) {
      dir *= -1;
      j = 0;
    }
    delay(100);
  }
}

void loop() {
}
