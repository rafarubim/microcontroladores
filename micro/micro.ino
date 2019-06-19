#define COLS 20
#define ROWS 4

#define null

#include "Lcd.hpp"
#include "Animation.hpp"

Lcd lcd = Lcd(COLS, ROWS);

int num_Measure = 10 ; // Set the number of measurements   
int pinSignal = A0; // pin connected to pin O module sound sensor   
long Sound_signal;    // Store the value read Sound Sensor   
long sum = 0 ; // Store the total value of n measurements   
long level = 0 ; // Store the average value
int isJumping = 0; // True (1) if character is jumping
unsigned long score;
unsigned long time_flag = 0; // Resets if player on the ground
float moment;

int pinguSize = 5;
int maxY = 31;
int minY = 0 + pinguSize;

float velocityY = 0;
int positionY = 31;     // Initial position of the character

void Update(float moment)
{
    positionY += velocityY * moment;
    positionY = floor(positionY/(maxY+1)*4);
}

void OnSoundLevelHigh(long level)
{
  velocityY = map(level, 0, 1021, 15, 50);
}

void OnSoundLevelLow()
{ 
  velocityY = 32;
}

void setup ()
{
  randomSeed(analogRead(A0));
  lcd.home();
  lcd.print("AAAAAAAAA
  
  
  ");
  //Animation::opening(lcd);
  
  pinMode(pinSignal, INPUT);
  Serial.begin (9600);
}
   
void loop ()  
{  
  // Performs 20 signal readings
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
    Sound_signal = analogRead (pinSignal);
    sum = sum + Sound_signal;
  }
 
  level = sum / num_Measure; // Calculate the average value
  if (level > 45) {
    Serial.println("Level: "+String(level));
    OnSoundLevelHigh(level);
  }
  else {
    OnSoundLevelLow();
  }

  if (positionY > maxY) { // Check maximun positionY
    positionY = maxY;
  } else if (positionY < minY) {
    positionY = minY;
  }
  
  moment = (millis() - time_flag)/1000;
  Update(moment);
  time_flag = millis();

  //lcd.setCursor(2,positionY);
  //lcd.write(PINGU);
  //lcd.clear();

  sum = 0 ; // Reset the sum of the measurement values
}
