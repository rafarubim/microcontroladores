#include "music.h"
#include <GFButton.h>
unsigned long previousMillis = 0;
int current = -1;
Note shootingstars[] = {{Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Dsharp5, 1500/2, 375/2},{Dsharp5, 250/2, 250/2},{E5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 500/2, 0},{Gsharp4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2},
                 {Fsharp5, 1500/2, 375/2},{Fsharp5, 250/2, 250/2},{Gsharp5, 500/2, 375/2},{Fsharp5, 500/2, 0},{Dsharp5, 500/2, 0},{B4, 250/2, 50/2}};

Note labyrinth[] = {{F2, 384, 0},{G2, 768, 1920},
                    {F2, 384, 0},{G2, 768, 768},
                    {Gsharp3, 768, 0},{F3, 384, 0},{C3, 384, 0},{D3, 384, 0},{F3, 1152, 384},
                    {C3, 384, 0},{F3, 384, 0}, {C4, 384, 0}, {Gsharp3, 768, 0}, {G3, 768, 1152}, {F3, 384, 0}, {G3, 768, 1920}};

bool outputTone = false;
const int tonePin = 5;
const int ground = A5;
bool tocar = false;
GFButton botao(A1);

void playpause(){
  Serial.println("press");
  tocar = !(tocar);
}

void setup() {
  Serial.begin(9600);
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);
  pinMode(tonePin, OUTPUT);
  botao.setPressHandler(playpause);
}

void loop() {
  botao.process();
  if(tocar){
    previousMillis = play(labyrinth, (sizeof(labyrinth)/sizeof(Note)) - 1 , previousMillis, current, outputTone, tonePin);
  }
  else {
    noTone(tonePin);
  }
}
