#ifndef MENUS_H
#define MENUS_H

#include "base.h"
#include "utilidades.h"
#include "desenho.h"
#include "arquivos.h"
#include "os-labirintos-do-inf.h"

void menuLabirinto(JOGO *);
void menuSalvarJogo(JOGO *);
void menuPrincipal(JOGO *);
void menuConfirmarSaida(JOGO *);
void menuNovojogo(JOGO *);
void menuPause(JOGO *);
void menuPergunta(JOGO *);
void menuCarregarJogo(JOGO *);
void menuInformacoes(JOGO *);
void menuEspecialSaindo(char []);
void menuAcabouTempo(JOGO *);
void menuFimDeJogo(JOGO *);
void menuTutorial(JOGO *);
void menuClassificacao(JOGO *);

#endif //MENUS_H