#include <vector>
#include <iostream>

const int tonePin = 8;
bool outputTone = false;

struct Note {
    unsigned int note;
    unsigned long noteDuration;
    unsigned long pauseBetweenNotes;
};

unsigned long play(std::vector<Note> melody, unsigned long previousMillis, int& current);