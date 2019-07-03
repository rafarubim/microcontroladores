#include "highscore.hpp"
#include <EEPROM.h>
#include <GFButton.h>
#include <LiquidCrystal.h>

#define qtdJogadores 8

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char main_word[4] = "AAA";
int CursorPoint = 0;
int endereco = 0;
int flag_end = 0;

GFButton UpButton = GFButton(A0, E_GFBUTTON_PULLDOWN);
GFButton LeftButton = GFButton(A1, E_GFBUTTON_PULLDOWN);
GFButton DownButton = GFButton(A2, E_GFBUTTON_PULLDOWN);
GFButton RightButton = GFButton(A4, E_GFBUTTON_PULLDOWN);

Highscore highscore;

Jogador jogador = {" ", 0};

void setup() {
  Serial.begin(9600);
  UpButton.setPressHandler(up_button_pressed);
  DownButton.setPressHandler(down_button_pressed);
  RightButton.setPressHandler(right_button_pressed);
  LeftButton.setPressHandler(left_button_pressed);
  highscore.endereco = endereco;
  get_score(highscore);
  jogador.pontos = 0.31;
  get_name();
//  highscore = score_max(highscore, jogador);
//  print_score(highscore);
}

void loop() {
  UpButton.process();
  DownButton.process();
  RightButton.process();
  LeftButton.process();
//  if(flag_end==0)get_name();
  if(flag_end==0)Serial.println(jogador.nome);
}

void down_button_pressed(){
   main_word[CursorPoint]++;
   if(main_word[CursorPoint] > 'Z'){
      main_word[CursorPoint] = 'Z';
    }
   lcd.print(main_word[CursorPoint]);
   lcd.setCursor(CursorPoint,2);
}


void up_button_pressed(){
   main_word[CursorPoint]--;
   if(main_word[CursorPoint] < 'A'){
      main_word[CursorPoint] = 'A';
    }
   lcd.print(main_word[CursorPoint]);
   lcd.setCursor(CursorPoint,2);
}

void left_button_pressed(){
   CursorPoint--;
   if(CursorPoint<0)CursorPoint=0;
   lcd.setCursor(CursorPoint,2);
}


void right_button_pressed(){
   CursorPoint++;
   if(CursorPoint>3)flag_end = 1;
   lcd.setCursor(CursorPoint,2);
}

void get_name(){
  int i = 0; 
  lcd.begin(20,4);
  lcd.print("Iniciais do seu nome");
  lcd.print(main_word);
  lcd.setCursor(CursorPoint,2);
  lcd.blink();
  if(flag_end==1){
    jogador.nome[0] = main_word[0];
    jogador.nome[0] = main_word[1];
    jogador.nome[0] = main_word[2];
    lcd.noBlink();
    return;
  }
}
