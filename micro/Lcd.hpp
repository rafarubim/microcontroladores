#pragma once

#include <LiquidCrystal.h>
#include <arduino.h>

static byte STAR(0);
static byte PINGU(1);
static byte STICK_MAN(2);

struct Pos {
  int x, y;
  Pos(int x, int y) : x(x), y(y) {}
  Pos() : Pos(0, 0) {}
};

class Lcd: public LiquidCrystal {
  const int _Rs = 12, _En = 11, _D4 = 5, _D5 = 4, _D6 = 3, _D7 = 2;
  int _cols, _rows;
  bool _leftToRight = true;
  bool _autoscroll = false;
  Pos _cursorPosBackup;
public:
  Pos cursorPos = {0, 0};
  Lcd(int cols, int rows);
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
  void moveCursor(int x, int y = 0);
  void clearCol(int col);
};
