#pragma once

#define qtdJogadores 8

struct jogador{
  char nome[4];
  int pontos;
};

typedef struct jogador Jogador;

struct highscore{
  Jogador jogador[qtdJogadores];
};

typedef struct highscore Highscore;

typedef bool (*ComparacaoJogadores)(Jogador, Jogador);

bool jogadorTemMaisPontos(Jogador j1, Jogador j2);

bool jogadorTemMenosPontos(Jogador j1, Jogador j2);

void print_score(int endereco);

bool isPlayerRecordist(int endereco, Jogador jogador, ComparacaoJogadores compararJogadores);

bool addPlayerRecord(int endereco, Jogador jogador, ComparacaoJogadores compararJogadores);

void clear_score(int endereco);

void setupGetPlayerName(int pontos);

void loopGetPlayerName();
