#include "Lcd.hpp"
#define uint8_t byte

static byte star[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b01010,
  0b10001,
  0b00000
};


static byte pingu[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b01110,
  0b01100,
  0b01100,
  0b01100,
  0b01010
};


static byte stickMan[8] = {
  0b00000,
  0b00100,
  0b00000,
  0b01110,
  0b10101,
  0b00100,
  0b01010,
  0b01010
};

Lcd::Lcd(int cols, int rows) : LiquidCrystal(_Rs, _En, _D4, _D5, _D6, _D7), _cols(cols), _rows(rows)  {
  begin(cols, rows);
  createChar(STAR, star);
  createChar(PINGU, pingu);
  createChar(STICK_MAN, stickMan);
  Pos currentPos(0, 0);
}

void Lcd::home() {
  LiquidCrystal::home();
  cursorPos.x = 0;
  cursorPos.y = 0;
}

void Lcd::setCursor(int col, int row) {
  LiquidCrystal::setCursor(col, row);
  cursorPos.x = col;
  cursorPos.y = row;
}

void Lcd::leftToRight() {
  LiquidCrystal::leftToRight();
  _leftToRight = true;
}

void Lcd::rightToLeft() {
  LiquidCrystal::rightToLeft();
  _leftToRight = false;
}

void Lcd::scrollDisplayLeft() {
  LiquidCrystal::scrollDisplayLeft();
  cursorPos.x--;
}

void Lcd::scrollDisplayRight() {
  LiquidCrystal::scrollDisplayRight();
  cursorPos.x++;
}

void Lcd::write(char c) {
  LiquidCrystal::write(c);
  _leftToRight ? cursorPos.x++ : cursorPos.x--;
}

void Lcd::print(String str) {
  LiquidCrystal::print(str);
  int len = str.length();
  cursorPos.x += _leftToRight ? len : -len;
}

void Lcd::moveCursor(int x, int y) {
  int newX = cursorPos.x + x;
  int newY = cursorPos.y + y;
  if (newX >= 0 && newX < _cols && newY >=0 && newY < _rows) {
    setCursor(newX, newY);
  }
}

void Lcd::ClearCol(int col) {
  Pos prevPos = cursorPos;
  for (int i = 0; i < _rows; i++) {
    setCursor(col, i);
    write(' ');
  }
  cursorPos = prevPos;
}
