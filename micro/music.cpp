#include "music.h"

unsigned long play(Note melody[], int N, unsigned long previousMillis, int& current, bool& outputTone, int tonePin) {
    unsigned long currentMillis = millis();
    if (current == -1) {
      outputTone = true;
      current++;
      tone(tonePin, melody[current].note, melody[current].noteDuration);
      return currentMillis;
    } else if (current <= N  ){
        if (outputTone) {
            if (currentMillis - previousMillis >= melody[current].noteDuration) {
                noTone(tonePin);
                outputTone = false;
                return currentMillis;
            }
        } else {
            if (currentMillis - previousMillis >= melody[current].pauseBetweenNotes) {
                outputTone = true;
                current++;
                tone(tonePin, melody[current].note, melody[current].noteDuration);
                return currentMillis;
            } 
        }
    }
    else {
      current = -1;
      return currentMillis;
    }
}
