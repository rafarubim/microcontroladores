#include "highscore.hpp"
#include "Lcd.hpp"
#include <EEPROM.h>
#include <GFButton.h>
#include <arduino.h>
#include "OptionsMenu.hpp"
#include "StateMachine.hpp"

#define null 0
#define qtdJogadores 8

struct Highscore {
  Jogador jogador[qtdJogadores];
};

typedef bool (*ComparacaoJogadores)(Jogador, Jogador);

static void save_score(int endereco, Highscore highscore);

static Highscore get_score(int endereco);

static void confirm();

static void getNameDownButtonPressed();

static void getNameUpButtonPressed();

static void getNameLeftButtonPressed();

static void getNameRightButtonPressed();

static void showHighscoreDownButtonPressed();

static void showHighscoreUpButtonPressed();

static void showHighscoreLeftButtonPressed();

static bool jogadorTemMaisPontos(Jogador j1, Jogador j2);

static bool jogadorTemMenosPontos(Jogador j1, Jogador j2);

static void detachButtons();

static char main_word[4] = "AAA";
static int CursorPoint = 0;
static int endereco = 0;
static int flag_end = 0;
static const int initialsY = 2;
static const int okX = 15;
static Jogador jogadorToGetName;
static OptionsMenu _optionsMenu = OptionsMenu();
static HighscoreTables _currentGame;

static GFButton UpButton = GFButton(A0, E_GFBUTTON_PULLUP);
static GFButton LeftButton = GFButton(A1, E_GFBUTTON_PULLUP);
static GFButton DownButton = GFButton(A2, E_GFBUTTON_PULLUP);
static GFButton RightButton = GFButton(A3, E_GFBUTTON_PULLUP);

static ComparacaoJogadores comparacoesTables[] = {
  jogadorTemMaisPontos, // SCREAM_JUMP
  jogadorTemMaisPontos, // SHOOTING_STARS
  jogadorTemMenosPontos // MAZE_CRAWLER
};

static int enderecosTables[] = {
  0*sizeof(Highscore), // SCREAM_JUMP
  1*sizeof(Highscore), // SHOOTING_STARS
  2*sizeof(Highscore) // MAZE_CRAWLER
};

static String nomesTables[] = {
  "Scream JUMP!", // SCREAM_JUMP
  "Shooting Stars", // SHOOTING_STARS
  "Maze Crawler" // MAZE_CRAWLER
};

bool isPlayerRecordist(HighscoreTables game, Jogador jogador) {
  int endereco = enderecosTables[game];
  ComparacaoJogadores compararJogadores = comparacoesTables[game];
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

bool addPlayerRecord(HighscoreTables game, Jogador jogador) {
  int endereco = enderecosTables[game];
  ComparacaoJogadores compararJogadores = comparacoesTables[game];
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

void clear_score(HighscoreTables game) {
  int endereco = enderecosTables[game];
  Highscore highscore = get_score(endereco);
  for (int i = 0; i< qtdJogadores ; i++) {
    highscore.jogador[i] = {{'\0','\0','\0','\0'},0};
  }
  save_score(endereco, highscore);
}

void setupGetPlayerName(HighscoreTables game, int pontos) {
  _currentGame = game;
  UpButton.setReleaseHandler(getNameUpButtonPressed);
  DownButton.setReleaseHandler(getNameDownButtonPressed);
  RightButton.setReleaseHandler(getNameRightButtonPressed);
  LeftButton.setReleaseHandler(getNameLeftButtonPressed);

  jogadorToGetName.pontos = pontos;
  CursorPoint = 0;
  main_word[0] = 'A';
  main_word[1] = 'A';
  main_word[2] = 'A';
  main_word[3] = '\0';
  Lcd& lcd = Lcd::getInstance();
  lcd.clear();
  lcd.home();
  lcd.print("Quebrou o recorde!");
  lcd.setCursor(0, 1);
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

void setupShowHighscore(HighscoreTables game) {
  int endereco = enderecosTables[game];
  Highscore highscore = get_score(endereco);
  _optionsMenu.addOption("Highscore de");
  _optionsMenu.addOption(nomesTables[game]);
  String scores[qtdJogadores];
  int playersAmount = 0;
  for (int i = 0; i < qtdJogadores; i++) {
    if (highscore.jogador[i].nome[0] != '\0') {
      playersAmount++;
      scores[i] = String(i+1) + "o: " + String(highscore.jogador[i].nome) + " - " + String(highscore.jogador[i].pontos);
    }
    else {
      break;
    }
  }
  _optionsMenu.createSelectionList(scores, playersAmount);

  UpButton.setReleaseHandler(showHighscoreUpButtonPressed);
  DownButton.setReleaseHandler(showHighscoreDownButtonPressed);
  LeftButton.setReleaseHandler(showHighscoreLeftButtonPressed);
}

void loopShowHighscore() {
  _optionsMenu.drawOptions();
  UpButton.process();
  DownButton.process();
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

static void getNameUpButtonPressed() {  
  Lcd& lcd = Lcd::getInstance();
  if (CursorPoint < 3) {
    main_word[CursorPoint]++;
    if (main_word[CursorPoint] > 'Z') {
      main_word[CursorPoint] = 'Z';
    }
    lcd.stamp(main_word[CursorPoint]);
  }
}

static void getNameDownButtonPressed() {
  Lcd& lcd = Lcd::getInstance();
  if (CursorPoint < 3) {
    main_word[CursorPoint]--;
    if(main_word[CursorPoint] < 'A'){
      main_word[CursorPoint] = 'A';
    }
    lcd.stamp(main_word[CursorPoint]);
  }
}

static void getNameLeftButtonPressed() {
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

  addPlayerRecord(_currentGame, jogadorToGetName);
  detachButtons();
  setupShowHighscore(_currentGame);
  changeState(SHOW_RECORDS);
}

static void getNameRightButtonPressed() {
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

static void showHighscoreDownButtonPressed() {
  _optionsMenu.moveSelectionDown();
}

static void showHighscoreUpButtonPressed() {
  _optionsMenu.moveSelectionUp();
}

static void showHighscoreLeftButtonPressed() {
  changeState(MENU);
}

static bool jogadorTemMaisPontos(Jogador j1, Jogador j2) {
  if (j1.pontos > j2.pontos) {
    return true;
  }
  return false;
}

static bool jogadorTemMenosPontos(Jogador j1, Jogador j2) {
  if (j1.pontos < j2.pontos) {
    return true;
  }
  return false;
}

static void detachButtons() {
  UpButton.setReleaseHandler(null);
  DownButton.setReleaseHandler(null);
  RightButton.setReleaseHandler(null);
  LeftButton.setReleaseHandler(null);
}
