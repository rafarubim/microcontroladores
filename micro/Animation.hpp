#pragma once
#include "Lcd.hpp"

template <typename T, unsigned S>
inline unsigned arraySize(const T(&v)[S]) {
  return S;
}

class Animation {
public:
  static void opening(Lcd lcd);
};
