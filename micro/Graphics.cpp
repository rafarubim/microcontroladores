#include "Graphics.hpp"
#include "Lcd.hpp"

#define null 0

Graphics* Graphics::_graphicsInstance = null;

Graphics::Graphics() {}

void Graphics::clearCharBuffer() {
  for (int i = 0; i < 8; i++) {
    _charBuffer[i] = 0;
  }
}

void Graphics::drawInCharBuffer(int currentRow, int currentCol, const byte (&drawing)[8], Pos pos) {
  for (int bitJ = 0; bitJ < 5; bitJ++) {
    for (int bitI = 0; bitI < 8; bitI++) {
      int xPos = pos.x + bitJ;
      int yPos = pos.y + bitI;
      int byteCol = xPos / 6;
      int byteRow = yPos / 9;
      if (byteCol == currentCol & byteRow == currentRow) {
        int bitCol = xPos % 6;
        int bitRow = yPos % 9;
        if (bitRow < 8 && bitCol < 5) {
          int bitIsOn = 0;
          bitIsOn = drawing[bitI] & (1 << (4 - bitJ));
          if (bitIsOn) {
            bitIsOn = 1;
          }
          _charBuffer[bitRow] |= bitIsOn << (4 - bitCol);
        }
      }
    }
  }
}

Graphics& Graphics::getInstance() {
  if (_graphicsInstance != null) {
    return *_graphicsInstance;
  }
  _graphicsInstance = new Graphics();
  return *_graphicsInstance;
}

double Graphics::getFps() {
  return _fps;
}

void Graphics::setFps(double fps) {
  _fps = fps;
  //_delayFrames = ceil(fps / _initialFps) - 1;
}

void Graphics::draw(const byte (&drawing)[8], Pos pos) {
  BufferedDrawing bufferedDrawing =  BufferedDrawing(drawing, Pos(pos.x, pos.y - 7), 1 + _delayFrames);
  pushElem(_drawingsBuffer, _drawingsAmount, bufferedDrawing);
}

void Graphics::stamp(char c, Pos pos) {
  Lcd lcd = Lcd::getInstance();
  int col = pos.x / 6;
  int row = pos.y / 9;
  lcd.stamp(c, col, row);
}

void Graphics::flushScreen() {
  Lcd lcd = Lcd::getInstance();
  int lcdCols = lcd.getCols();
  int lcdRows = lcd.getRows();
  for (int j = 0; j < lcdCols; j++) {
    for (int i = 0; i < lcdRows; i++) {
      clearCharBuffer();

      for (int k = 0; k < _drawingsAmount; k++) {
        BufferedDrawing bufferedDrawing = _drawingsBuffer[k];
        
        drawInCharBuffer(i, j, bufferedDrawing.drawing, bufferedDrawing.pos);
      }
      bool isBufferEmpty = true;
      for (int i = 0; i < 8; i++) {
        if (_charBuffer[i]) {
          isBufferEmpty = false;
          break;
        }
      }
      if (isBufferEmpty) {
        lcd.stamp(' ', j, i);
      } else {
        lcd.stampTemp(_charBuffer, j, i);
      }
    }
  }
  for (int k = 0; k < _drawingsAmount; k++) {
    _drawingsBuffer[k].framesLeft--;
    if (_drawingsBuffer[k].framesLeft <= 0) {
      removeElem(_drawingsBuffer, _drawingsAmount, k);
      k--;
    }
  }
}

void Graphics::processGraphics() {
  unsigned long now = millis();
  if (now - _lastFlush >= 1000 / _fps) {
    _lastFlush = now;
    flushScreen();
  }
}
