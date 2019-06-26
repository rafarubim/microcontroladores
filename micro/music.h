#include<Arduino.h>

#define F2 87
#define G2 98
#define C3 131
#define D3 147
#define F3 175
#define G3 196
#define Gsharp3 208
#define C4 262
#define D4 294
#define F4 349
#define G4 392
#define Gsharp4 415
#define B4 494
#define C5 523
#define Dsharp5 622
#define E5 659
#define Fsharp5 740
#define Gsharp5 831

struct Note {
    unsigned int note;
    unsigned long noteDuration;
    unsigned long pauseBetweenNotes;
};

unsigned long play(Note melody[], int N, unsigned long previousMillis, int& current, bool& outputTone, int tonePin);
