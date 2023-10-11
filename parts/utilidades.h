#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "base.h"

void preencherVazioAleatorioCom(char [TAM_LAB][TAM_LAB], char);
int carregaTextura(Texture2D *, char *);
void alterarItemSelecionado(int *, int);
Vector2 vectorAdd(Vector2, Vector2);
Vector2 vectorSub(Vector2, Vector2);
Vector2 vectorMult(Vector2, Vector2);
int obterQuadrante(Vector2, int, int);
void mudarCena(JOGO *, int);
void ordernarRanking(RANKING [], int);
int vectorEqual(Vector2, Vector2);
Vector2 vectorMultEscalar(Vector2, float);
Vector2 vectorAddEscalar(Vector2, float);
Vector2 obterInicioLabirinto(char [TAM_LAB][TAM_LAB]);
void atualizarTempo(JOGO *);
int menorIgual(float, float, float);
int contarCreditosDiponiveis(PROFESSOR [], int, char [TAM_LAB][TAM_LAB]);
void carregarJogo(JOGO *, SAVE);

#endif //UTILIDADES_H