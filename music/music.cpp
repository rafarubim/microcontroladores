#include "music.h"

unsigned long play(std::vector<Note> melody, unsigned long previousMillis, int& current) {
    unsigned long currentMillis = millis();

    if (outputTone) {
        if (currentMillis - previousMillis >= melody[current].noteDuration) {
            previousMillis = currentMillis;
            noTone(tonePin);
            outputTone = false;
            return previousMillis;
        }
    } else {
        if (currentMillis - previousMillis >= melody[current].pauseBetweenNotes) {
            previousMillis = currentMillis;
            tone(tonePin, melody[current].note);
            outputTone = true;

            if (current == melody.size() - 1)
                current = 0;
            else
                current++;

            return previousMillis;
        }
    }
}
// Use:
// std::vector<Note> melody;
// previousMillis = 0;
// loop:
// previousMillis = play(array_de_struct_tipo_note, previousMillis, current);