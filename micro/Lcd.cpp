#include "Lcd.hpp"

#define null 0
#define uint8_t byte

Lcd* Lcd::_lcdInstance = null;
int Lcd::_defaultCols = 0;
int Lcd::_defaultRows = 0;

Lcd::Lcd(int cols, int rows): LiquidCrystal(_Rs, _Rw, _En, _D4, _D5, _D6, _D7), _cols(cols), _rows(rows) {
  digitalWrite(_Rw, LOW);
  begin(cols, rows);
}

void Lcd::configLcd(int cols, int rows) {
  _defaultCols = cols;
  _defaultRows = rows;
}

Lcd* Lcd::getInstance() {
  if (_lcdInstance != null) {
    return _lcdInstance;
  }
  _lcdInstance = new Lcd(_defaultCols, _defaultRows);
  return _lcdInstance;
}

int Lcd::getCols() {
  return _cols;
}

int Lcd::getRows() {
  return _rows;
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

void Lcd::pushCursorPos() {
  _cursorPosBackup = cursorPos;
}

void Lcd::popCursorPos() {
  cursorPos = _cursorPosBackup;
}

void Lcd::stamp(char c, int x, int y) {
  if (x >= 0 && x < _cols && y >=0 && y < _rows) {
    pushCursorPos();
    setCursor(x, y);
    write(c);
    popCursorPos();
  }
}

void Lcd::stamp(char c, int x) {
  stamp(c, x, cursorPos.y);
}

void Lcd::stamp(char c) {
  stamp(c, cursorPos.x);
}


void Lcd::stampTemp(byte temp[8], int x, int y) {
  createChar(_currentTempChar, temp);
  stamp(_currentTempChar, x, y);
  _currentTempChar++;
  if (_currentTempChar >= MAX_TEMPS) {
    _currentTempChar = 0;
  }
}

void Lcd::stampTemp(byte temp[8], int x) {
  stampTemp(temp, x, cursorPos.y);
}

void Lcd::stampTemp(byte temp[8]) {
  stampTemp(temp, cursorPos.x);
}

void Lcd::moveCursor(int x, int y) {
  int newX = cursorPos.x + x;
  int newY = cursorPos.y + y;
  if (newX >= 0 && newX < _cols && newY >=0 && newY < _rows) {
    setCursor(newX, newY);
  }
}

void Lcd::clearCol(int col) {
  pushCursorPos();
  for (int i = 0; i < _rows; i++) {
    setCursor(col, i);
    write(' ');
  }
  popCursorPos();
}
