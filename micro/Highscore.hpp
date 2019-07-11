#pragma once

#define qtdJogadores 8

struct Jogador{
  char nome[4];
  int pontos;
};

enum HighscoreTables {
  SCREAM_JUMP_TABLE,
  SHOOTING_STARS_TABLE,
  MAZE_CRAWLER_TABLE
};

bool jogadorTemMaisPontos(Jogador j1, Jogador j2);

bool jogadorTemMenosPontos(Jogador j1, Jogador j2);

bool isPlayerRecordist(HighscoreTables game, Jogador jogador);

bool addPlayerRecord(HighscoreTables game, Jogador jogador);

void clear_score(HighscoreTables game);

void setupGetPlayerName(int pontos);

void loopGetPlayerName();

void setupShowHighscore(HighscoreTables game);

void loopShowHighscore();
