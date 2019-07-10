#include "OptionsMenu.hpp"
#include "Lcd.hpp"

static byte ARROW = 0b01111110;

void OptionsMenu::updateSelectionArray() {
  int maxSelectionAmount = MAX_OPTIONS - _selectionArrayOptionInx;
  for (int i = 0; i < _selectionAmount && i < maxSelectionAmount; i++) {
    String prefix = "  ";
    if (_currentSelection == i) {
      prefix = " ";
    }
    _optionsArray[_selectionArrayOptionInx + i] = prefix + _selectionArray[i];
  }
  Lcd lcd = Lcd::getInstance();
  int selectionRow = _currentSelection + _selectionArrayOptionInx - _currentScroll;
  if (selectionRow < 0) {
    _currentScroll += selectionRow;
    lcd.clear();
  } else if (selectionRow >= lcd.getRows()) {
    _currentScroll += selectionRow - lcd.getRows() + 1;
    lcd.clear();
  }
}

OptionsMenu::OptionsMenu() {}

void OptionsMenu::clearOptions() {
  _optionsAmount = 0;
}

void OptionsMenu::addOption(String option) {
  if (_optionsAmount == 0) {
    _currentScroll = 0;
  }
  _optionsArray[_optionsAmount] = option;
  _optionsAmount++;
}

void OptionsMenu::moveSelectionUp() {
  if (_currentSelection >= 0) {
    _currentSelection--;
    if (_currentSelection < 0) {
      _currentSelection = 0;
    }
    updateSelectionArray();
  }
}

void OptionsMenu::moveSelectionDown() {
  if (_currentSelection >= 0) {
    _currentSelection++;
    if (_currentSelection >= _selectionAmount) {
      _currentSelection = _selectionAmount - 1;
    }
    updateSelectionArray();
  }
}

int OptionsMenu::getSelectionInx() {
  return _currentSelection;
}

void OptionsMenu::drawOptions() {
  Lcd& lcd = Lcd::getInstance();
  for (int i = _currentScroll; i < _optionsAmount && i < _currentScroll + lcd.getRows(); i++) {
    int row = i - _currentScroll;
    lcd.setCursor(0, row);
    if (_currentSelection >= 0) {
      int selectionRow = _currentSelection + _selectionArrayOptionInx - _currentScroll;
      if (row == selectionRow) {
        lcd.write(ARROW);
      }
    }
    lcd.print(_optionsArray[i]);
  }
}
