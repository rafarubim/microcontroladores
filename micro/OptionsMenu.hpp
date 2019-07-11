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
  void createSelectionList(String(&selectionArray)[N], int n = N) {
    for (int i = 0; i < n; i++) {
      _selectionArray[i] = selectionArray[i];
    }
    _selectionAmount = n;
    _selectionArrayOptionInx = _optionsAmount;
    _currentSelection = 0;
    _optionsAmount += n;
    updateSelectionArray();
  }
  void moveSelectionUp();
  void moveSelectionDown();
  int getSelectionInx();
  void drawOptions();
};
