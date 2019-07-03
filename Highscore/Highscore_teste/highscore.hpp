#pragma once

#define qtdJogadores 8

struct jogador{
  char nome[4];
  float pontos;
};

typedef struct jogador Jogador;

struct highscore{
  Jogador jogador[qtdJogadores];
  int endereco;
};

typedef struct highscore Highscore;

void save_score(Highscore highscore);

void print_score(Highscore highscore);

Highscore score_max(Highscore highscore, Jogador jogador);

Highscore score_min(Highscore highscore, Jogador jogador);

void clear_score(Highscore highscore);

void get_score(Highscore highscore);
