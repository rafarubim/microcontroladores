#include "highscore.hpp"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <arduino.h>

#define qtdJogadores 8

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);

void save_score(Highscore highscore){
  EEPROM.put(highscore.endereco,highscore);
}

void print_score(Highscore highscore){
  lcd2.begin(20,4);
  for (int i =0; i< qtdJogadores ; i++){
    lcd2.print(i+1);
    lcd2.print('-');
    lcd2.print(highscore.jogador[i].nome);
    lcd2.print('-');
    lcd2.print(highscore.jogador[i].pontos);
    delay(1000);
    lcd2.clear();
  }
}


Highscore score_max(Highscore highscore, Jogador jogador){
    for (int i =0; i< qtdJogadores ; i++){
      if (jogador.pontos > highscore.jogador[i].pontos) {
        for(int k = qtdJogadores-1; k > i; k++) {
          highscore.jogador[k].pontos = highscore.jogador[k-1].pontos;
          highscore.jogador[k].nome[0] = highscore.jogador[k-1].nome[0];
          highscore.jogador[k].nome[1] = highscore.jogador[k-1].nome[1];
          highscore.jogador[k].nome[2] = highscore.jogador[k-1].nome[2];
        }
      highscore.jogador[i].pontos = jogador.pontos;
      EEPROM.put(highscore.endereco,highscore);
      break;
    }
  }
  return highscore;
}

Highscore score_min(Highscore highscore, Jogador jogador){
    for (int i =0; i< qtdJogadores ; i++){
      if (jogador.pontos < highscore.jogador[i].pontos) {
        for(int k = qtdJogadores-1; k > i; k++) {
          highscore.jogador[k].pontos = highscore.jogador[k-1].pontos;
          highscore.jogador[k].nome[0] = highscore.jogador[k-1].nome[0];
          highscore.jogador[k].nome[1] = highscore.jogador[k-1].nome[1];
          highscore.jogador[k].nome[2] = highscore.jogador[k-1].nome[2];
        }
      highscore.jogador[i].pontos = jogador.pontos;
      EEPROM.put(highscore.endereco,highscore);
      break;
    }
  }
  return highscore;
}

void clear_score(Highscore highscore){
  for (int i =0; i< qtdJogadores ; i++){
    highscore.jogador[i] = {0,0};
  }
  EEPROM.put(highscore.endereco,highscore);
}

void get_score(Highscore highscore){
   EEPROM.get(highscore.endereco,highscore);
}
