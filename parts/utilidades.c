#include "utilidades.h"
/*
Funções utilitárias diversas
*/


// Dado um labirinto e um caractere, preenche um espaço vazio aleatório com o
// caractere

void preencherVazioAleatorioCom(char lab[TAM_LAB][TAM_LAB], char preencher) {
    int i, j;

    do {
        i = GetRandomValue(0, TAM_LAB - 1);
        j = GetRandomValue(0, TAM_LAB - 1);

    } while (lab[i][j] != ' ');

    lab[i][j] = preencher;
}


// Carrega uma textura, dado seu endereco e um ponteiro para a textura
int carregaTextura(Texture2D *dest, char *file_name) {
    int sucesso = 1;
    Image image =
        LoadImage(file_name);  // Load image data into CPU memory (RAM)

    if (image.width <= 0) sucesso = 0;

    *dest = LoadTextureFromImage(
        image);  // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);

    return sucesso;
}


// Verifica se foi pressionado uma tecla para mudar o item selecionado e
// então muda
void alterarItemSelecionado(int *itemSelecionado, int qtdeItems) {
    // Verifica se foi pressionado uma tecla para mudar o item selecionado;

    // Se for apertado para cima ou para esquerda, e o item selecionado for
    // maior que zero, diminui
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT)) &&
        (*itemSelecionado) > 0)
        (*itemSelecionado)--;

    // Se for apertado para baixo ou para direita, e o item selecionado for
    // menor que a quantidade de items, aumenta
    else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT)) &&
             (*itemSelecionado) < (qtdeItems - 1))
        (*itemSelecionado)++;
}


Vector2 vectorAdd(Vector2 v1, Vector2 v2) {
    Vector2 v3 = {v1.x + v2.x, v1.y + v2.y};
    return v3;
}

Vector2 vectorSub(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x - v2.x, v1.y - v2.y};
}

Vector2 vectorMult(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x * v2.x, v1.y * v2.y};
}

// Obtem o quadrante de um ponto em relação a um retangulo
int obterQuadrante(Vector2 p, int width, int height) {
    int r = -1;
    if (p.x >= width / 2) {
        if (p.y >= height / 2)
            r = 4;
        else
            r = 1;
    } else {
        if (p.y >= height / 2)
            r = 3;
        else
            r = 2;
    }
    return r;
}


// Muda a cena atual
void mudarCena(JOGO *jogo, int cena) {
    int a;
    if (cena == -1) {
        // Volta a cena anterior
        a = jogo->cenaAtual;
        jogo->cenaAtual = jogo->cenaAnterior;
        jogo->cenaAnterior = a;
    } else {
        // Muda de cena
        jogo->cenaAnterior = jogo->cenaAtual;
        jogo->cenaAtual = cena;
        jogo->itemSelecionado = 0;
        PlaySound(jogo->sons.select);
    }
}

// Ordena a lista de jogadores
void ordernarRanking(RANKING lista[], int qtde) {
    int i, sorted, fim;
    RANKING aux;

    do {
        // Flag que está organizados
        sorted = 1;

        // Bubble sort
        for (i = 0; i < qtde - 1; i++) {
            // Muda de posição SE tiver pontuação menor
            // OU se a pontuação for igual e for lexicamente anterior
            if (lista[i].pontuacao < lista[i + 1].pontuacao ||
                (lista[i].pontuacao == lista[i + 1].pontuacao &&
                 strcmp(lista[i].nome, lista[i + 1].nome) > 0)) {
                // Inverte os itens
                aux = lista[i];
                lista[i] = lista[i + 1];
                lista[i + 1] = aux;

                sorted = 0;
            }
        }
    } while (!sorted);
}


int vectorEqual(Vector2 v1, Vector2 v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

Vector2 vectorMultEscalar(Vector2 v, float e) {
    return (Vector2){v.x * e, v.y * e};
}

Vector2 vectorAddEscalar(Vector2 v, float e) {
    return (Vector2){v.x + e, v.y + e};
}

Vector2 obterInicioLabirinto(char lab[TAM_LAB][TAM_LAB]) {
    int i = 0, j = 0;

    for (i = 0; i < TAM_LAB; i++)
        for (j = 0; j < TAM_LAB; j++)
            if (lab[i][j] == 'I') return (Vector2){j, i};
}




void atualizarTempo(JOGO *jogo) {
    jogo->jogador.tempoMapa += GetFrameTime();
    jogo->jogador.tempoTotal += GetFrameTime();

    jogo->tempoRestante = jogo->tempoMaximo - jogo->jogador.tempoMapa;
    if (jogo->tempoRestante <= 0) {
        mudarCena(jogo, CENA_ACABOU_TEMPO);
    }
}


// Verifica se um número está entre dois outros
int menorIgual(float a, float b, float c) { return ((a <= b) && (b <= c)); }


// Conta quantos créditos estão disponíveis no labirinto, considerando os
// itens e professores
int contarCreditosDiponiveis(PROFESSOR professores[], int qtdeP,
                             char lab[TAM_LAB][TAM_LAB]) {
    int qtde = 0, x, y;

    // Conta os créditos no labirinto
    for (x = 0; x < TAM_LAB; x++) {
        for (y = 0; y < TAM_LAB; y++) {
            if (lab[y][x] == 'C') qtde++;
        }
    }

    // Conta os créditos dos professores não respondidos
    for (int x = 0; x < qtdeP; x++) {
        if (!professores[x].respondido) qtde++;
    }

    return qtde;
}

void carregarJogo(JOGO *jogo, SAVE save) {
    int i, j;

    jogo->qtdeProfessores = save.qtdeProfessores;
    jogo->qtdeColegas = save.qtdeColegas;
    jogo->qtdeBombasAtivas = save.qtdeBombasAtivas;
    jogo->nivel = save.nivel;
    jogo->dificuldade = save.dificuldade;
    jogo->creditosNecessarios = save.creditosNecessarios;
    jogo->tempoMaximo = save.tempoMaximo;
    jogo->tempoRestante = save.tempoRestante;
    jogo->jogador = save.jogador;

    for (i = 0; i < save.qtdeBombasAtivas; i++)
        jogo->bombasAtivas[i] = save.bombasAtivas[i];

    for (i = 0; i < save.qtdeProfessores; i++)
        jogo->professores[i] = save.professores[i];

    for (i = 0; i < save.qtdeColegas; i++) jogo->colegas[i] = save.colegas[i];

    for (i = 0; i < TAM_LAB; i++)
        for (j = 0; j < TAM_LAB; j++)
            jogo->labirintoAtual[i][j] = save.labirintoAtual[i][j];
}
