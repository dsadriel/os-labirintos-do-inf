#ifndef OS-LABIRINTOS-DO-INF_H
#define OS-LABIRINTOS-DO-INF_H

#include "base.h"

void desenharjogoPrincipal(JOGO *);
void popularLabirinto(JOGO *);
void popularProfessores(JOGO *, int);
void atualizarProfessores(JOGO *);
int carregarRecursos(JOGO *);
void descarregarRecursos(JOGO *);
int carregarTexturas(TEXTURESLIST *);
int carregarSons(SOUNDLIST *);
void iniciarLabirinto(JOGO *);
void atualizarJogador(JOGO *);
int irEmDirecaoAojogador(PROFESSOR *, JOGADOR *, char [TAM_LAB][TAM_LAB]);
int colocarBomba(Vector2, BOMBA [], int *);
void atualizarBombas(JOGO *);
void explodirBomba(JOGO *, BOMBA);
void carregarJogo(JOGO *, SAVE);

#endif //OS-LABIRINTOS-DO-INF_H