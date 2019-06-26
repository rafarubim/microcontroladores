#pragma once
#include "Pos.h"
#include "arrayUtils.h"
#include <arduino.h>

#define drawingsBufferSize 10

struct BufferedDrawing {
  byte drawing[8];
  Pos pos;
  int framesLeft;
  BufferedDrawing(const byte (&_drawing)[8], Pos pos, int framesLeft):
    pos(pos), framesLeft(framesLeft) {
    arrayCopy(drawing, _drawing);
  }
  BufferedDrawing(): BufferedDrawing({0,0,0,0,0,0,0,0}, Pos(), 0) {}
};

class Graphics {
  static Graphics* _graphicsInstance;
  static const int _initialFps = 5;
  double _fps = _initialFps;
  int _delayFrames = 5;
  unsigned long _lastFlush = 0;
  BufferedDrawing _drawingsBuffer[drawingsBufferSize];
  Graphics();
  int _drawingsAmount = 0;
  byte _charBuffer[8];
  void clearCharBuffer();
  void drawInCharBuffer(int currentRow, int currentCol, const byte (&drawing)[8], Pos pos);
public:
  static Graphics& getInstance();
  double getFps();
  void setFps(double fps);
  void draw(const byte (&drawing)[8], Pos pos);
  void stamp(char c, Pos pos);
  
  void flushScreen();
  void processGraphics();
};
