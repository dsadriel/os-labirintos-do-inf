#include "desenho.h"
/*
Todas as funções de desenho e axuliares para desenho estão aqui
*/

// Desenha o labirinto, incluindo todos os itens, professores, colegas e jogador
void desenharLabirinto(JOGO *jogo) {
    int x, y, i;
    float escalaBomba;
    Vector2 p;

    BeginMode2D(jogo->camera);

    for (x = 0; x < TAM_LAB; x++) {
        for (y = 0; y < TAM_LAB; y++) {
            p = (Vector2){x * BLOCK_SIZE, y * BLOCK_SIZE};

            // Sempre desenha uma textura "livre" por baixo
            DrawTextureAutoScale(jogo->texturas.livre, p, 0, BLOCK_SIZE, WHITE);

            switch (jogo->labirintoAtual[y][x]) {
                case '.':
                    DrawTextureAutoScale(jogo->texturas.parede, p, 0,
                                         BLOCK_SIZE, DARKGRAY);
                    break;
                case '#':
                    DrawTextureAutoScale(jogo->texturas.parede, p, 0,
                                         BLOCK_SIZE, jogo->corParede);
                    break;
                case 'R':
                    DrawTextureAutoScale(jogo->texturas.relogio, p, 0,
                                         BLOCK_SIZE, WHITE);
                    break;
                case 'C':
                    DrawTextureAutoScale(jogo->texturas.credito, p, 0,
                                         BLOCK_SIZE, WHITE);
                    break;
                case 'V':
                    DrawTextureAutoScale(jogo->texturas.vida, p, 0, BLOCK_SIZE,
                                         WHITE);
                    break;
                case 'B':
                    DrawTextureAutoScale(jogo->texturas.bombaItem, p, 0,
                                         BLOCK_SIZE, WHITE);
                    break;
                case 'S':
                    DrawTextureAutoScale(jogo->texturas.saida, p, 0, BLOCK_SIZE,
                                         WHITE);
                    break;
                case 'c':
                    desenharSprite(
                        vectorMultEscalar((Vector2){x, y}, BLOCK_SIZE),
                        (Vector2){0, 0}, 1.5, jogo->texturas.colega, WHITE);
                    break;
            }
        }
    }

    // Desenha os professores
    desenharProfessores(jogo->professores, jogo->qtdeProfessores,
                        jogo->texturas.professor);

    // Desenha os colegas
    desenharColegas(jogo->colegas, jogo->qtdeColegas, jogo->texturas.colega);

    // Desenhas as bombas
    for (i = 0; i < jogo->qtdeBombasAtivas; i++) {
        escalaBomba = ((int)GetTime() % 2 == 0 ? 1.5 : 1.25);

        p = vectorMultEscalar(jogo->bombasAtivas[i].pos, BLOCK_SIZE);

        DrawTextureAutoScale(jogo->texturas.bombaAtiva, p, 0,
                             escalaBomba * BLOCK_SIZE,
                             ((int)GetTime() % 2 == 0 ? WHITE : RED));
    }

    // Desenha o jogador
    desenharSprite(vectorMultEscalar(jogo->jogador.pos, BLOCK_SIZE),
                   jogo->jogador.dpos, 1.5, jogo->texturas.jogador, WHITE);

    EndMode2D();
}


// Desenha os professores no labirinto

void desenharProfessores(PROFESSOR professores[], int qtde, Texture2D textura) {
    int i;
    for (i = 0; i < qtde; i++) {
        if (!professores[i].respondido) {
            desenharSprite(vectorMultEscalar(professores[i].pos, BLOCK_SIZE),
                           professores[i].dpos, 1.5, textura, WHITE);
        }
    }
}


// Desenha a barra lateral, necessita que o modo de desenho tenha sido iniciado
void desenharBarraLateral(JOGO *jogo) {
    int meioSidebar = BLOCK_SIZE * 56;
    Color corTempoRestante;

    // Desenha a barra lateral
    DrawRectangle(BLOCK_SIZE * 50, 0, BLOCK_SIZE * 12, BLOCK_SIZE * 50, BLACK);

    // Labirinto atual
    DrawTextCenter("Labirinto", meioSidebar, BLOCK_SIZE * 1, BLOCK_SIZE * 2,
                   GRAY);
    DrawTextCenter(TextFormat("%d", jogo->nivel), meioSidebar, BLOCK_SIZE * 4,
                   BLOCK_SIZE * 4, WHITE);

    // Tempo restante
    switch ((int)jogo->tempoRestante) {
        case 0 ... 60:
            corTempoRestante = (int)(GetTime() * 10) % 5 == 0
                                   ? RED
                                   : (Color){255, 0, 0, 255};
            break;
        case 61 ... 120:
            corTempoRestante = RED;
            break;
        default:
            corTempoRestante = COLOR_GOLD;
    }
    DrawTextCenter(TextFormat("%.2d:%.2d", (int)jogo->tempoRestante / 60,
                              (int)jogo->tempoRestante % 60),
                   meioSidebar, BLOCK_SIZE * 10, BLOCK_SIZE * 3,
                   corTempoRestante);
    DrawTextCenter("Tempo restante", meioSidebar, BLOCK_SIZE * 13,
                   BLOCK_SIZE * 1, GRAY);

    // Creditos
    DrawTextCenter(TextFormat("%d/%d", jogo->jogador.creditosMapa,
                              jogo->creditosNecessarios),
                   meioSidebar, BLOCK_SIZE * 16, (int)(BLOCK_SIZE * 2.5),
                   COLOR_GOLD);
    DrawTextCenter("Créditos", meioSidebar, BLOCK_SIZE * 19, BLOCK_SIZE * 1,
                   GRAY);

    // Bombas
    DrawTextCenter(TextFormat("%d", jogo->jogador.bombas), meioSidebar,
                   BLOCK_SIZE * 22, BLOCK_SIZE * 3, COLOR_DARK_BROWN);
    DrawTextCenter("Bombas", meioSidebar, BLOCK_SIZE * 25, BLOCK_SIZE * 1,
                   GRAY);

    // Vidas
    DrawTextCenter(TextFormat("%d", jogo->jogador.vidas), meioSidebar,
                   BLOCK_SIZE * 28, BLOCK_SIZE * 3, RED);
    DrawTextCenter("Vidas", meioSidebar, BLOCK_SIZE * 31, BLOCK_SIZE * 1, GRAY);

    // Pontuação
    DrawTextCenter(TextFormat("%d", jogo->jogador.pontuacao), meioSidebar,
                   BLOCK_SIZE * 33, BLOCK_SIZE * 2.5, COLOR_LIGHT_BLUE);
    DrawTextCenter("Pontuação", meioSidebar, BLOCK_SIZE * 36, BLOCK_SIZE * .8,
                   GRAY);

    // Minimapa
    desenharMiniMapa(jogo->labirintoAtual, jogo->jogador.pos, BLOCK_SIZE * 50,
                     BLOCK_SIZE * 38, BLOCK_SIZE * 12);
}


// Desnhar minimapa
void desenharMiniMapa(char lab[TAM_LAB][TAM_LAB], Vector2 posjogador, int posX,
                      int posY, int size) {
    float tamCelula = (float)size / TAM_LAB;
    int x, y, q;

    // Desenha o fundo minimapa
    DrawRectangle(posX, posY, size, size, GRAY);

    // Desenha as celulas
    for (x = 0; x < TAM_LAB; x++) {
        for (y = 0; y < TAM_LAB; y++) {
            switch (lab[y][x]) {
                case '#':
                    DrawRectangle(x * tamCelula + posX, y * tamCelula + posY,
                                  ceil(tamCelula), ceil(tamCelula), BLACK);
                    break;
                case 'V':
                    DrawRectangle(x * tamCelula + posX, y * tamCelula + posY,
                                  ceil(tamCelula), ceil(tamCelula), RED);
                    break;

                case 'C':
                    DrawRectangle(x * tamCelula + posX, y * tamCelula + posY,
                                  ceil(tamCelula), ceil(tamCelula), COLOR_GOLD);
                    break;
                case 'R':
                    DrawRectangle(x * tamCelula + posX, y * tamCelula + posY,
                                  ceil(tamCelula), ceil(tamCelula), BLUE);
                    break;
                case 'B':
                    DrawRectangle(x * tamCelula + posX, y * tamCelula + posY,
                                  ceil(tamCelula), ceil(tamCelula),
                                  COLOR_DARK_BROWN);
                    break;
                case 'S':
                    DrawRectangle((x - 1.5) * tamCelula + posX,
                                  (y - 1.5) * tamCelula + posY, tamCelula * 3,
                                  tamCelula * 4, YELLOW);
                    break;
            }
        }
    }

    // Esconde os quadrantes em que o jogador não está
    q = obterQuadrante(posjogador, TAM_LAB, TAM_LAB);
    if (q != 1)
        DrawRectangle(posX + size / 2, posY, size / 2, size / 2,
                      (Color){0, 0, 0, 165});
    if (q != 2)
        DrawRectangle(posX, posY, size / 2, size / 2, (Color){0, 0, 0, 165});
    if (q != 3)
        DrawRectangle(posX, posY + size / 2, size / 2, size / 2,
                      (Color){0, 0, 0, 165});
    if (q != 4)
        DrawRectangle(posX + size / 2, posY + size / 2, size / 2, size / 2,
                      (Color){0, 0, 0, 165});

    // Desenha o a representação do jogador
    DrawRectangle((posjogador.x - 2) * tamCelula + posX,
                  (posjogador.y - 2) * tamCelula + posY, tamCelula * 4,
                  tamCelula * 4, GREEN);
}


// Desenha os colegas no labirinto
void desenharColegas(COLEGA colegas[], int qtde, Texture2D textura) {
    int i;
    for (i = 0; i < qtde; i++) {
        if (!colegas[i].respondido) {
            desenharSprite(vectorMultEscalar(colegas[i].pos, BLOCK_SIZE),
                           (Vector2){0, 0}, 1.5, textura, WHITE);
        }
    }
}

// Desenha o texto centralizado na coordenada X
void DrawTextCenter(char texto[], int x, int y, int size, Color cor) {
    int dx = MeasureText(texto, size) / 2;

    DrawText(texto, x - dx, y, size, cor);
}

// Desenha o texto limitado a um retangulo e devolve ultima posição desenhada
Vector2 DrawTextRec(char txt[], Rectangle box, int fontSize, Color textColor) {
    char *palavras[MAX_TEXTO], *p, texto[MAX_TEXTO];
    int qtdePalavras, i, dx;
    Vector2 referencia = {box.x, box.y};

    // Copia o texto para uma variavel local
    strcpy(texto, txt);

    // Separa as palavras
    qtdePalavras = 0;
    p = strtok(texto, " ");

    // Enquanto houver palavras
    while (p != NULL) {
        // Adicona a palavra na lista e incrementa o contador
        palavras[qtdePalavras] = p;
        qtdePalavras++;

        // Pega a proxima palavra
        p = strtok(NULL, " ");
    }

    // Desenha as palavras
    for (i = 0; i < qtdePalavras; i++) {
        // Calcula o tamanho da palavra
        dx = MeasureText(palavras[i], fontSize);

        // Se a palavra não couber na linha, pula para a proxima
        if ((referencia.x + dx) > (box.x + box.width)) {
            referencia.x = box.x;
            referencia.y += fontSize;
        }

        // Desenha a palavra
        DrawText(palavras[i], referencia.x, referencia.y, fontSize, textColor);

        // Atualiza a referencia
        referencia.x += dx + MeasureText(" ", fontSize);
    }

    // Retorna a ultima posição desenhada
    return referencia;
}

// Desenha uma lista de texto em um retangulo
Vector2 DrawTextRecList(char texto[][MAX_OPTLEN], int lines, Rectangle box,
                        int fontSize, Color textColorActive,
                        Color textColorInactive, int selected) {
    int i;
    Rectangle referencia = box;
    Vector2 ultimo = {0, 0};

    // Desenha as linhas
    for (i = 0; i < lines; i++) {
        ultimo =
            DrawTextRec(texto[i], referencia, fontSize,
                        selected == i ? textColorActive : textColorInactive);

        // Atualiza a referencia para a proxima linha com um espaçamento de 10%
        referencia.y = ultimo.y + (fontSize * 1.1);
    }
}

// Desenha uma lista de texto centralizado nas coorendas x e y
void DrawTextCenterList(char texto[][MAX_OPTLEN], int lines, int x, int y,
                        int fontSize, Color textColorActive,
                        Color textColorInactive, int selected) {
    int i, dy;

    for (i = 0; i < lines; i++) {
        dy = ((lines / 2.0) - i) * fontSize;
        // Desenha o texto centralizado
        DrawTextCenter(texto[i], x, y - dy, fontSize,
                       selected == i ? textColorActive : textColorInactive);
    }
}


// Desenha um sprite animado
void desenharSprite(Vector2 pos, Vector2 dpos, float scale, Texture2D sprite,
                    Color color) {
    Rectangle source, position;
    Vector2 origin;

    if (dpos.y > 0)
        source =
            (Rectangle){SPRITE_WIDTH * ((int)(GetTime() * SPRITE_SPEED) % 3), 0,
                        SPRITE_WIDTH, SPRITE_HEIGHT};
    else if (dpos.y < 0)
        source =
            (Rectangle){SPRITE_WIDTH * ((int)(GetTime() * SPRITE_SPEED) % 3),
                        SPRITE_HEIGHT * 2, SPRITE_WIDTH, SPRITE_HEIGHT};
    else if (dpos.x > 0)
        // Inverte a textura
        source = (Rectangle){
            SPRITE_WIDTH * ((int)(GetTime() * SPRITE_SPEED) % 3) - SPRITE_WIDTH,
            SPRITE_HEIGHT, -SPRITE_WIDTH, SPRITE_HEIGHT};
    else if (dpos.x < 0)
        source =
            (Rectangle){SPRITE_WIDTH * ((int)(GetTime() * SPRITE_SPEED) % 3),
                        SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
    else
        source = (Rectangle){0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};

    position.x = pos.x;
    position.y = pos.y;

    position.width = BLOCK_SIZE * scale;
    position.height = sprite.height * position.width / sprite.width;

    origin.x = (position.width - BLOCK_SIZE) / 2;
    origin.y = (position.height - BLOCK_SIZE);

    DrawTexturePro(sprite, source, position, origin, 0, color);
}



// Desenha uma textura com a largunra maxima informada
void DrawTextureAutoScale(Texture2D texture, Vector2 position, float rotation,
                          float maxWidth, Color tint) {
    Rectangle source = {0.0f, 0.0f, (float)texture.width,
                        (float)texture.height};
    Rectangle dest = {position.x, position.y, (float)maxWidth,
                      (float)texture.height * maxWidth / texture.width};
    Vector2 origin = {0.0f,
                      (texture.height * maxWidth / texture.width) - maxWidth};

    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}


void desenharItemTutorial(char nome[], char texto[], Texture2D textura,
                          Vector2 origem) {
    DrawTexturePro(textura, (Rectangle){0, 0, textura.width, textura.height},
                   (Rectangle){origem.x, origem.y, 64, 64}, (Vector2){0, 0},
                   0.0, WHITE);
    DrawText(nome, origem.x + 80, origem.y, 25, WHITE);
    DrawTextRec(texto, (Rectangle){origem.x + 80, origem.y + 35, 300, 100}, 20,
                GRAY);
}

void desenharSpriteTutorial(char nome[], char texto[], Texture2D textura,
                            Vector2 origem, int animacao) {
    desenharSprite((Vector2){origem.x - 90, origem.y + 120},
                   (Vector2){0, animacao}, 7, textura, WHITE);
    DrawText(nome, origem.x, origem.y, 25, WHITE);
    DrawTextRec(texto, (Rectangle){origem.x, origem.y + 35, 300, 100}, 20,
                GRAY);
}