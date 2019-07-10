#pragma once
#include "Pos.h"
#include "arrayUtils.h"
#include <arduino.h>

#define maxSimultaneousDrawings 10
#define drawingsBufferSize 30
#define stampsBufferSize 20

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

struct BufferedStamp {
  char c;
  byte b;
  bool isChar;
  byte representation[8];
  Pos pos;
  BufferedStamp(char c, const byte (&_representation)[8], Pos pos): pos(pos), c(c), isChar(true) {
    arrayCopy(representation, _representation);
  }
  BufferedStamp(byte b, const byte (&_representation)[8], Pos pos): pos(pos), b(b), isChar(false) {
    arrayCopy(representation, _representation);
  }
  BufferedStamp(): BufferedStamp('\0', {0,0,0,0,0,0,0,0}, Pos()) {}
};

class Graphics {
  static Graphics* _graphicsInstance;
  static const int _initialFps = 20;
  double _fps = _initialFps;
  int _delayFrames = 0;
  unsigned long _lastFlush = 0;
  BufferedDrawing _drawingsDefined[maxSimultaneousDrawings];
  BufferedDrawing _drawingsBuffer[drawingsBufferSize];
  BufferedStamp _stampsBuffer[stampsBufferSize];
  int _definedDrawingsAmount = 0;
  int _drawingsAmount = 0;
  int _stampsAmount = 0;
  Graphics();
  byte _charBuffer[8];
  void clearCharBuffer();
  void drawInCharBuffer(int currentRow, int currentCol, const byte (&drawing)[8], Pos pos);
  void drawDefinedDrawings();
public:
  static Graphics& getInstance();
  double getFps();
  void setFps(double fps);
  void draw(const byte (&drawing)[8], Pos pos);
  void stamp(char c, const byte (&representation)[8], Pos pos);
  
  void flushScreen();
  void processGraphics();
};
