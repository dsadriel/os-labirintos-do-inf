#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "base.h"
#include "utilidades.h"
#include "os-labirintos-do-inf.h"

void carregarLabirinto(JOGO *);
void carregarPerguntas(PERGUNTA [], int *);
int carregarClassificacao(RANKING []);
int salvarJogadorComoColega(JOGADOR, int);
int salvarColegas(COLEGA [], int);
int carregarColegas(COLEGA [], int, char [TAM_LAB][TAM_LAB]);
int salvarJogo(JOGO *);
int carregarSaves(SAVE []);
SAVE gerarSave(JOGO *);
int salvarClassificacao(JOGADOR, int, int *, char []);
int removerColegaDoArquivo(COLEGA);

#endif //ARQUIVOS_H