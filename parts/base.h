#ifndef BASE_H
#define BASE_H

/*
Aqui estão definidas as constantes e estruturas utilizadas no jogo.
*/


// Bibliotecas necssárias para o jogo
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*

 ██████  ██████  ███    ██ ███████ ████████  █████  ███S    ██ ████████ ███████
██      ██    ██ ████   ██ ██         ██    ██   ██ ████   ██    ██    ██
██      ██    ██ ██ ██  ██ ███████    ██    ███████ ██ ██  ██    ██    █████
██      ██    ██ ██  ██ ██      ██    ██    ██   ██ ██  ██ ██    ██    ██
 ██████  ██████  ██   ████ ███████    ██    ██   ██ ██   ████    ██    ███████


*/

#define ALTURA_TELA 900
#define LARGURA_TELA 1044

#define BLOCK_SIZE 16
#define BOMBA_RAIO_EXPLOSAO 3   // Em blocos
#define BOMBA_TEMPO_EXPLOSAO 5  // Em segundos
#define DEBUG_USERNAME "DEBUG********************"
#define FILE_COLEGAS "./data/colegas.bin"
#define FILE_LABIRINTOS "./data/labirintos.bin"
#define FILE_PERGUNTAS "./data/perguntas.txt"
#define FILE_RANKING "./data/ranking.bin"
#define FILE_SAVES "./data/saves.bin"
#define MAX_BOMBAS_ATIVAS 10
#define MAX_COLEGAS_SALVOS 100
#define MAX_CREDITOS_LAB 50
#define MAX_LABS 100
#define MAX_NOME_JOGADOR 25
#define MAX_OPTLEN 150
#define MAX_OPTS 6
#define MAX_PROFESSORES_POR_LAB 50
#define MAX_RANKING 100
#define MAX_SAVES 100
#define MAX_TEXTO 300
#define MAX_PERGUNTAS 500
#define SPRITE_HEIGHT 30
#define SPRITE_SPEED 10  // Inversamente proporcional
#define SPRITE_WIDTH 24
#define TAM_LAB 100

#define COLOR_GOLD \
    (Color) { 237, 176, 0, 255 }
#define COLOR_DARK_BROWN \
    (Color) { 74, 63, 61, 255 }
#define COLOR_GREEN \
    (Color) { 0, 141, 82, 255 }
#define COLOR_LIGHT_BLUE \
    (Color) { 188, 226, 229, 255 }

/*
███████ ████████ ██████  ██    ██  ██████ ████████ ███████
██         ██    ██   ██ ██    ██ ██         ██    ██
███████    ██    ██████  ██    ██ ██         ██    ███████
     ██    ██    ██   ██ ██    ██ ██         ██         ██
███████    ██    ██   ██  ██████   ██████    ██    ███████
*/

typedef struct {
    Texture2D aluno;
    Texture2D colega;
    Texture2D credito;
    Texture2D livre;
    Texture2D bomba;
    Texture2D bombaItem;
    Texture2D bombaAtiva;
    Texture2D parede;
    Texture2D professor;
    Texture2D relogio;
    Texture2D saida;
    Texture2D vida;
    Texture2D jogador;
    Texture2D explosao;
    Texture2D disquete;
} TEXTURESLIST;  // Lista de texturas do jogo

typedef struct {
    Sound hit;
    Sound select;
    Sound pickup;
    Sound powerup;
    Sound select_m;
    Sound blocked;
    Sound correctAnswer;
    Sound wrongAnswer;
    Sound timesUp;
    Sound gameOver;
    Sound levelUp;
    Sound explosion;

} SOUNDLIST;  // Lista de sons do jogo

typedef struct {
    char pergunta[MAX_TEXTO + 1];   // Título da pergunta
    int n_alternativas;   // Número de alternativas
    int resposta_correta;  // Resposta correta
    char alternativas[MAX_OPTS][MAX_OPTLEN];  // Alternativas
    int ja_respondida;  // Se já foi respondida pelo jogador
} PERGUNTA;

typedef struct {
    Vector2 pos;     // Posição do professor
    Vector2 dpos;    // Direção do professor
    int respondido;  // Se já foi respondido pelo jogador
} PROFESSOR;

typedef struct {
    Vector2 pos;  // Posição do colega
    int nivel;  // Nível em que o colega foi salvo. -1 representa que o colega
                // não foi salvo e sim gerado aleatoriamente
    int respondido;  // Se já foi respondido pelo jogador
} COLEGA;

typedef struct {
    Vector2 pos;       // Posição da bomba
    double explodeEm;  // Tempo em que a bomba explode, calculado a partir do
                       // tempo atual + tempo de explosão
} BOMBA;

typedef struct {
    char nome[MAX_NOME_JOGADOR + 1];  // Nome do jogador
    int vidas;                        // Vidas do jogador
    int bombas;                       // Bombas que o jogador possui

    float tempoMapa;   // Tempo que o jogador está no mapa atual
    float tempoTotal;  // Tempo total que o jogador jogou até o instante atual

    int creditosMapa;   // Créditos que o jogador coletou no mapa atual
    int creditosTotal;  // Créditos que o jogador coletou até o instante atual

    Vector2 pos;   // Posição do jogador
    Vector2 dpos;  // Direção do jogador

    int pontuacao;  // Pontuação do jogador
} JOGADOR;

typedef struct {
    char nome[MAX_NOME_JOGADOR + 1];  // Nome do jogador
    int pontuacao;                    // Pontuação do jogador
    int dificuldade;                  // Dificuldade em que o jogador jogou

} RANKING;


typedef struct {
    PROFESSOR professores[MAX_PROFESSORES_POR_LAB];
    int qtdeProfessores;
    COLEGA colegas[MAX_COLEGAS_SALVOS];
    int qtdeColegas;

    char labirintoAtual[TAM_LAB][TAM_LAB];
    BOMBA bombasAtivas[MAX_BOMBAS_ATIVAS];
    int qtdeBombasAtivas;
    int nivel;
    int dificuldade;
    int creditosNecessarios;
    int tempoMaximo;
    float tempoRestante;

    JOGADOR jogador;

} SAVE; //Os campos possui as mesmas representações do que a struct JOGO

typedef struct {
    int cenaAtual;              // Representa a cena atual do jogo
    int cenaAnterior;           // Representa a cena anterior do jogo
    int itemSelecionado;        // Representa o item selecionado pelo jogador,
                                // utilizado em menus
    int sair;                   // Se o jogador deseja sair do jogo
    char mensagem[MAX_OPTLEN];  // Mensagem a ser exibida na tela de saída
    Camera2D camera;            // Câmera do jogo

    TEXTURESLIST texturas;  // Texturas do jogo
    SOUNDLIST sons;         // Sons do jogo
    Music musica;           // Música do jogo
    float volumeMusica;     // Volume da música

    PROFESSOR
        professores[MAX_PROFESSORES_POR_LAB];  // Representa os professores do
                                               // labirinto atual, o vetor é
                                               // inicializado a cada novo
                                               // labirinto
    int qtdeProfessores;  // Quantidade de professores no labirinto atual
    COLEGA colegas[MAX_COLEGAS_SALVOS];  // Representa os colegas do labirinto,
                                         // o vetor é inicializado a cada novo
                                         // labirinto
    int qtdeColegas;      // Quantidade de colegas no labirinto atual
    PERGUNTA perguntas[MAX_PERGUNTAS];  // Representa as perguntas do jogo, o vetor é
                          // inicializado ao iniciar o jogo
    int qtdePerguntas;    // Quantidade de perguntas do jogo
    int perguntaAtual;    // Índice da pergunta atual
    char origemPergunta;  // Representa a origem da pergunta - 'P': Professor,
                          // 'c' : Colega
    Vector2 origemPerguntaPos;  // Posição da origem da pergunta

    char labirintoAtual[TAM_LAB]
                       [TAM_LAB];  // Matriz que representa o labirinto atual
    BOMBA bombasAtivas[MAX_BOMBAS_ATIVAS];  // Representa as bombas ativas no
                                            // labirinto atual
    Color corParede;                        // Cor da parede do labirinto atual
    int qtdeBombasAtivas;  // Quantidade de bombas ativas no labirinto atual
    int qtdeLabirintos;  // Quantidade de labirintos do jogo, informação obtida
                         // do arquivo binário
    int nivel;        // Nível atual do jogo
    int dificuldade;  // Dificuldade do jogo, informação obtida do usuário no
                      // menu
    int creditosNecessarios;  // Créditos necessários para passar de nível
    int tempoMaximo;          // Tempo máximo para passar de nível
    float tempoRestante;      // Tempo restante para passar de nível

    RANKING ranking[MAX_RANKING];  // Vetor que representa o ranking do jogo,
                                   // informação obtida do arquivo binário
    int qtdeRanks;                 // Quantidade de jogadores no ranking

    SAVE saves[MAX_SAVES];  // Vetor que representa os saves do jogo, informação
                            // obtida do arquivo binário
    int qtdeSaves;          // Quantidade de saves do jogo

    JOGADOR jogador;  // Representa o jogador
} JOGO;


enum CENAS {
    CENA_MENU_PRINCIPAL,
    CENA_SAIR,
    CENA_MENU_NOVOJOGO1,
    CENA_MENU_NOVOJOGO2,
    CENA_MENU_CARREGAR,
    CENA_MENU_SALVAR,
    CENA_MENU_INFORMACOES,
    CENA_MENU_PAUSE,
    CENA_PERGUNTA,
    CENA_LABIRINTO,
    CENA_ACABOU_TEMPO,
    CENA_FIM_JOGO,
    CENA_MENU_CLASSIFICACAO,
    MENU_TUTORIAL
};

#endif //BASE_H