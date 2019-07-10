#include "highscore.hpp"
#include "Lcd.hpp"
#include <EEPROM.h>
#include <GFButton.h>
#include <arduino.h>

#define null 0
#define qtdJogadores 8

static char main_word[4] = "AAA";
static int CursorPoint = 0;
static int endereco = 0;
static int flag_end = 0;
static const int initialsY = 1;
static const int okX = 15;
static Jogador jogadorToGetName;

static GFButton UpButton = GFButton(A0, E_GFBUTTON_PULLUP);
static GFButton LeftButton = GFButton(A1, E_GFBUTTON_PULLUP);
static GFButton DownButton = GFButton(A2, E_GFBUTTON_PULLUP);
static GFButton RightButton = GFButton(A4, E_GFBUTTON_PULLUP);

static void save_score(int endereco, Highscore highscore);

static Highscore get_score(int endereco);

static void down_button_pressed();

static void up_button_pressed();

static void left_button_pressed();

static void confirm();

static void right_button_pressed();

void print_score(int endereco) {
  Highscore highscore = get_score(endereco);
  Lcd& lcd = Lcd::getInstance();
  for (int i = 0; i< qtdJogadores ; i++) {
    lcd.setCursor(0, 0);
    lcd.print(String(i+1));
    lcd.print(" -> ");
    lcd.print(highscore.jogador[i].nome);
    lcd.print(" -> ");
    lcd.print(String(highscore.jogador[i].pontos));
    delay(1000);
    lcd.clear();
  }
}

bool jogadorTemMaisPontos(Jogador j1, Jogador j2) {
  if (j1.pontos > j2.pontos) {
    return true;
  }
  return false;
}

bool jogadorTemMenosPontos(Jogador j1, Jogador j2) {
  if (j1.pontos < j2.pontos) {
    return true;
  }
  return false;
}

bool isPlayerRecordist(int endereco, Jogador jogador, ComparacaoJogadores compararJogadores) {
    Highscore highscore = get_score(endereco);
    for (int i = 0; i < qtdJogadores ; i++) {
      if (highscore.jogador[i].nome[0] == '\0') {
        return true;
      }
      if (compararJogadores(jogador, highscore.jogador[i])) {
      return true;
    }
  }
  return false;
}

bool addPlayerRecord(int endereco, Jogador jogador, ComparacaoJogadores compararJogadores) {
    Highscore highscore = get_score(endereco);
    for (int i = 0; i < qtdJogadores ; i++) {
      if (highscore.jogador[i].nome[0] == '\0') {
        highscore.jogador[i] = jogador;
        save_score(endereco, highscore);
        return true;
      }
      if (compararJogadores(jogador, highscore.jogador[i])) {
        for(int k = qtdJogadores-1; k > i; k--) {
          highscore.jogador[k].pontos = highscore.jogador[k-1].pontos;
          highscore.jogador[k].nome[0] = highscore.jogador[k-1].nome[0];
          highscore.jogador[k].nome[1] = highscore.jogador[k-1].nome[1];
          highscore.jogador[k].nome[2] = highscore.jogador[k-1].nome[2];
        }
      highscore.jogador[i] = jogador;
      save_score(endereco, highscore);
      return true;
    }
  }
  return false;
}

void clear_score(int endereco) {
  Highscore highscore = get_score(endereco);
  for (int i = 0; i< qtdJogadores ; i++) {
    highscore.jogador[i] = {{'\0'},0};
  }
  save_score(endereco, highscore);
}

void setupGetPlayerName(int pontos) {
  UpButton.setPressHandler(up_button_pressed);
  DownButton.setPressHandler(down_button_pressed);
  RightButton.setPressHandler(right_button_pressed);
  LeftButton.setPressHandler(left_button_pressed);

  jogadorToGetName.pontos = pontos;
  CursorPoint = 0;
  main_word[0] = 'A';
  main_word[1] = 'A';
  main_word[2] = 'A';
  main_word[3] = '\0';
  Lcd& lcd = Lcd::getInstance();
  lcd.clear();
  lcd.home();
  lcd.print("Iniciais do seu nome");
  lcd.setCursor(0, initialsY);
  lcd.print(main_word);
  lcd.setCursor(okX, initialsY);
  lcd.print("OK? >");
  lcd.setCursor(CursorPoint, initialsY);
  lcd.blink();
}

void loopGetPlayerName() {
  UpButton.process();
  DownButton.process();
  RightButton.process();
  LeftButton.process();
}

static void save_score(int endereco, Highscore highscore) {
  EEPROM.put(endereco, highscore);
}

static Highscore get_score(int endereco) {
   Highscore highscore;
   EEPROM.get(endereco,highscore);
   return highscore;
}

static void down_button_pressed() {
  Lcd& lcd = Lcd::getInstance();
  if (CursorPoint < 3) {
    main_word[CursorPoint]++;
    if (main_word[CursorPoint] > 'Z') {
      main_word[CursorPoint] = 'Z';
    }
    lcd.stamp(main_word[CursorPoint]);
  }
}


static void up_button_pressed(){
  Lcd& lcd = Lcd::getInstance();
  if (CursorPoint < 3) {
    main_word[CursorPoint]--;
    if(main_word[CursorPoint] < 'A'){
      main_word[CursorPoint] = 'A';
    }
    lcd.stamp(main_word[CursorPoint]);
  }
}

static void left_button_pressed() {
  Lcd& lcd = Lcd::getInstance();
  CursorPoint--;
  if(CursorPoint<0)CursorPoint=0;
  lcd.setCursor(CursorPoint,initialsY);
}

static void confirm() {
  Lcd& lcd = Lcd::getInstance();
  jogadorToGetName.nome[0] = main_word[0];
  jogadorToGetName.nome[1] = main_word[1];
  jogadorToGetName.nome[2] = main_word[2];
  jogadorToGetName.nome[3] = main_word[3];
  lcd.noBlink();
}

static void right_button_pressed() {
  Lcd& lcd = Lcd::getInstance();
  CursorPoint++;
  if(CursorPoint>=4) {
    confirm();
    return;
  }
  if (CursorPoint < 3) {
    lcd.setCursor(CursorPoint,initialsY);
  } else {
    lcd.setCursor(okX + 4,initialsY);
  }
}
