#pragma once
#include <arduino.h>

#define MAX_OPTIONS 8
#define MAX_SELECTIONS 8

class OptionsMenu {
  String _optionsArray[MAX_OPTIONS];
  int _optionsAmount = 0;
  int _currentScroll = -1;
  int _selectionArrayOptionInx = -1;
  String _selectionArray[MAX_SELECTIONS];
  int _selectionAmount = 0;
  int _currentSelection = -1;
  void updateSelectionArray();
public:
  OptionsMenu();
  void clearOptions();
  void addOption(String option);
  template <unsigned N>
  void createSelectionList(String(&selectionArray)[N]) {
    for (int i = 0; i < N; i++) {
      _selectionArray[i] = selectionArray[i];
    }
    _selectionAmount = N;
    _selectionArrayOptionInx = _optionsAmount;
    _currentSelection = 0;
    _optionsAmount += N;
    updateSelectionArray();
  }
  void moveSelectionUp();
  void moveSelectionDown();
  void drawOptions();
};
