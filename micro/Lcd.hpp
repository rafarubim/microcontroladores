#pragma once

#include <LiquidCrystal.h>
#include "Pos.h"
#include <arduino.h>

#define MAX_TEMPS 8

static byte FLOATING_X =0b11101011;

class Lcd: public LiquidCrystal {
  static const int _Rs = 12, _En = 11, _D4 = 5, _D5 = 4, _D6 = 3, _D7 = 2, _Rw = 8;
  static int _defaultCols, _defaultRows;
  static Lcd* _lcdInstance;
  int _cols, _rows;
  byte _currentTempChar = 0;
  bool _leftToRight = true;
  bool _autoscroll = false;
  Pos _cursorPosBackup;
  Lcd(int cols, int rows);
public:
  static void configLcd(int cols, int rows);
  static Lcd& getInstance();
  Pos cursorPos = {0, 0};

  int getCols();
  int getRows();
  void home();
  void setCursor(int col, int row);
  void leftToRight();
  void rightToLeft();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void write(char c);
  void print(String str);
  void pushCursorPos();
  void popCursorPos();
  void stamp(char c, int x, int y);
  void stamp(char c, int x);
  void stamp(char c);
  void stamp(byte b, int x, int y);
  void stamp(byte b, int x);
  void stamp(byte b);
  void stampTemp(byte temp[8], int x, int y);
  void stampTemp(byte temp[8], int x);
  void stampTemp(byte temp[8]);
  void moveCursor(int x, int y = 0);
  void clearCol(int col);
};
