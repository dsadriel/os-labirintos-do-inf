#ifndef DESENHO_H
#define DESENHO_H

#include "base.h"
#include "utilidades.h"

void desenharLabirinto(JOGO *);
void desenharProfessores(PROFESSOR [], int, Texture2D);
void desenharBarraLateral(JOGO *);
void desenharMiniMapa(char [TAM_LAB][TAM_LAB], Vector2, int, int, int);
void desenharColegas(COLEGA [], int, Texture2D);
void DrawTextCenter(char [], int, int, int, Color);
Vector2 DrawTextRec(char [], Rectangle, int, Color);
Vector2 DrawTextRecList(char [][MAX_OPTLEN], int, Rectangle, int, Color, Color, int);
void DrawTextCenterList(char [][MAX_OPTLEN], int, int, int, int, Color, Color, int);
void desenharSprite(Vector2, Vector2, float, Texture2D, Color);
void DrawTextureAutoScale(Texture2D, Vector2, float, float, Color);
void desenharItemTutorial(char [], char [], Texture2D, Vector2);
void desenharSpriteTutorial(char [], char [], Texture2D, Vector2, int);

#endif //DESENHO_H