#pragma once

struct Pos {
  int x, y;
  Pos(int x, int y) : x(x), y(y) {}
  Pos() : Pos(0, 0) {}
};
