/*******************************************************************************************
TRABALHO FINAL
ALGORITMOS E PROGRAMAÇÃO
TURMA A - 2023/1

********************************************************************************************/

#include "base.h" // Importa as bibliotecas padrão e define as constantes/structs
#include "utilidades.h" // Importa as funções utilitárias
#include "menus.h" // Importa as funções dos menus
#include "arquivos.h" // Importa as funções de manipulação de arquivos
#include "os-labirintos-do-inf.h" // Importa as funções do jogo principal

int main() {
    // Inicializações
    InitWindow(992, 800, "Os Labirintos do INF");

    InitAudioDevice();     // Inicializa o dispositivo de audio
    SetTargetFPS(30);      // Define a taxa de atualização da tela
    JOGO jogo = {0};       // Inicializa a struct jogo zerada
    jogo.camera.zoom = 1;  // Zoom da camera
    jogo.corParede = COLOR_DARK_BROWN;  // Cor inicial das paredes
    jogo.volumeMusica = 0.3f;           // Volume da musica
    SetExitKey(-1);  // Desabilita a tecla ESC para sair do jogo
    HideCursor();    // Esconde o cursor

    // Carregamento de recursos
    if (!carregarRecursos(&jogo)) {
        // Se ocorreu algum erro ao carregar os recursos, encerra o jogo
        jogo.cenaAtual = -1;
        strcpy(jogo.mensagem, "Erro ao carregar recursos!");
    }

    // Carrega as classificações do arquivo binário
    jogo.qtdeRanks = carregarClassificacao(jogo.ranking);

    // Inicia a musica de fundo
    PlayMusicStream(jogo.musica);
    SetMusicVolume(jogo.musica, jogo.volumeMusica);

    // Loop do jogo
    while ((!WindowShouldClose()) && !jogo.sair) {
        // Atualiza a musica de fundo
        UpdateMusicStream(jogo.musica);

        if (jogo.cenaAtual != CENA_MENU_NOVOJOGO1) {
            // Verifica se o jogador quer aumentar ou diminuir o volume da
            // musica
            if (IsKeyPressed(KEY_KP_SUBTRACT))
                SetMusicVolume(jogo.musica, (jogo.volumeMusica -= 0.1));
            if (IsKeyPressed(KEY_KP_ADD))
                SetMusicVolume(jogo.musica, (jogo.volumeMusica += 0.1));

            if (jogo.volumeMusica < 0)
                jogo.volumeMusica = 0.0;
            else if (jogo.volumeMusica > 1)
                jogo.volumeMusica = 1;

            // Verifica se o jogador quer mutar ou desmutar a musica
            if (IsKeyPressed(KEY_M)) {
                jogo.volumeMusica = jogo.volumeMusica == 0.0 ? 0.3 : 0.0;
                SetMusicVolume(jogo.musica, jogo.volumeMusica);
            }
        }

        // Atualiza o jogo
        desenharjogoPrincipal(&jogo);
    }

    // Fecha o dispositivo de audio
    CloseAudioDevice();

    menuEspecialSaindo(jogo.mensagem);  // Exibe a mensagem saída
    descarregarRecursos(&jogo);                // Encerra o jogo

    WaitTime(5);    // Espera 5 segundos
    CloseWindow();  // Fecha a janela

    TraceLog(LOG_INFO, "Jogo encerrado com sucesso!");

    return 0;
}

// Desenhar o jogo, função redireciona para a cena a ser desenhada
void desenharjogoPrincipal(JOGO *jogo) {
    switch (jogo->cenaAtual) {
        case CENA_SAIR:
            menuConfirmarSaida(jogo);
            break;
        case CENA_MENU_PRINCIPAL:
            menuPrincipal(jogo);
            break;
        case CENA_MENU_NOVOJOGO1:
            menuNovojogo(jogo);
            break;
        case CENA_MENU_NOVOJOGO2:
            menuNovojogo(jogo);
            break;
        case CENA_MENU_CARREGAR:
            menuCarregarJogo(jogo);
            break;
        case CENA_MENU_INFORMACOES:
            menuInformacoes(jogo);
            break;
        case CENA_MENU_PAUSE:
            menuPause(jogo);
            break;
        case CENA_LABIRINTO:
            menuLabirinto(jogo);
            break;
        case CENA_PERGUNTA:
            menuPergunta(jogo);
            break;
        case CENA_ACABOU_TEMPO:
            menuAcabouTempo(jogo);
            break;
        case CENA_FIM_JOGO:
            menuFimDeJogo(jogo);
            break;
        case MENU_TUTORIAL:
            menuTutorial(jogo);
            break;
        case CENA_MENU_CLASSIFICACAO:
            menuClassificacao(jogo);
            break;
        case CENA_MENU_SALVAR:
            menuSalvarJogo(jogo);
            break;
    }
}

// Popular o labirinto com itens, professores, créditos, etc
void popularLabirinto(JOGO *jogo) {
    int i, qCreditos, qProfessores, qVidas, qRelogios, qBombas;

    // Calcula a quantidade de CREDITOS que serão espalhados pelo labirinto
    qCreditos = 4 + jogo->nivel * (jogo->dificuldade + 1.5) +
                (jogo->dificuldade - 1) * 3 +
                GetRandomValue(0, jogo->dificuldade - 1) * 4;

    qCreditos = fmin(qCreditos, TAM_LAB * 5);
    jogo->creditosNecessarios = qCreditos = fmin(qCreditos, MAX_CREDITOS_LAB);

    // Calcula a quantidade de professores
    qProfessores = fmin((int)(qCreditos * (jogo->dificuldade / 5.0) + 4),
                        MAX_PROFESSORES_POR_LAB);

    // Atualiza a quantidade de créditos
    qCreditos = (int)(qCreditos * 0.75);

    // Calcula a quantidade de VIDAS que serão espalhados pelo laibirinto
    qVidas = 10 / jogo->nivel + GetRandomValue(0, 5 - jogo->dificuldade) * 2;
    qVidas = fmax(3 - jogo->dificuldade, qVidas);
    if (qVidas < 0 || jogo->dificuldade == 4) qVidas = 0;

    // Calcula a quantidade de RELÓGIOS que serão espalhados pelo labirinto
    qRelogios = 10 / jogo->nivel + GetRandomValue(0, 5 - jogo->dificuldade) * 2;
    qRelogios = fmax(3 - jogo->dificuldade, qVidas);
    if (qRelogios < 0 || jogo->dificuldade == 4) qVidas = 1;

    // Calcula a quantidade de bombas
    qBombas = qRelogios / 2;

    // Popula os professores e carregar os colegas
    popularProfessores(jogo, qProfessores);
    jogo->qtdeColegas =
        carregarColegas(jogo->colegas, jogo->nivel, jogo->labirintoAtual);

    // Insierir os itens no labirinto
    for (i = 0; i < qCreditos; i++)
        preencherVazioAleatorioCom(jogo->labirintoAtual, 'C');

    for (i = 0; i < qRelogios; i++)
        preencherVazioAleatorioCom(jogo->labirintoAtual, 'R');

    for (i = 0; i < qVidas; i++)
        preencherVazioAleatorioCom(jogo->labirintoAtual, 'V');

    for (i = 0; i < qBombas; i++)
        preencherVazioAleatorioCom(jogo->labirintoAtual, 'B');
}

// Popular os professores no labirinto
void popularProfessores(JOGO *jogo, int qtde) {
    int i, x, y;
    Vector2 dpos;

    // Atualiza a quantidade de professores
    jogo->qtdeProfessores = qtde;

    // Preeneche o vetor de professores
    for (i = 0; i < qtde; i++) {
        // Encontra um espaço vazio aleatório
        do {
            x = GetRandomValue(0, TAM_LAB - 1);
            y = GetRandomValue(0, TAM_LAB - 1);
        } while (jogo->labirintoAtual[y][x] != ' ');

        // Define um deslocamento aleatório
        dpos.x = GetRandomValue(-1, 1);
        if (dpos.x == 0) {
            dpos.y = GetRandomValue(-1, 1);
            if (dpos.y == 0) dpos.y = 1;
        }

        // Insere o professor no vetor
        jogo->professores[i] = (PROFESSOR){(Vector2){x, y}, dpos, 0};
    }
}


// Atualiza a posição dos professores
void atualizarProfessores(JOGO *jogo) {
    int i, k, temVisaoComJogador;
    Vector2 posPretendida, novaDirecao;

    for (i = 0; i < jogo->qtdeProfessores; i++) {
        // Só executa se o professor está ativo
        if (!jogo->professores[i].respondido) {
            // Faz com que o professor ande em direção ao jogador, se possui
            // linha de visão
            irEmDirecaoAojogador(&jogo->professores[i], &jogo->jogador,
                                 jogo->labirintoAtual);

            posPretendida =
                vectorAdd(jogo->professores[i].pos, jogo->professores[i].dpos);

            // Se a posição pretendida for uma parede, somente atualiza a
            // direção
            if (jogo->labirintoAtual[(int)posPretendida.y]
                                    [(int)posPretendida.x] == '#' ||
                jogo->labirintoAtual[(int)posPretendida.y]
                                    [(int)posPretendida.x] == '.') {
                // Escolhe uma nova direção aleatória
                k = 0;  // Numero de tentativas
                do {
                    switch (GetRandomValue(0, 3)) {
                        case 0:
                            novaDirecao = (Vector2){-1, 0};
                            break;
                        case 1:
                            novaDirecao = (Vector2){1, 0};
                            break;
                        case 2:
                            novaDirecao = (Vector2){0, 1};
                            break;
                        case 3:
                            novaDirecao = (Vector2){0, -1};
                            break;
                    }
                    k++;

                    // Enquanto a nova direção for uma parade ou tiver feito 10
                    // tentativas
                } while (
                    (jogo->labirintoAtual[(int)(jogo->professores[i].pos.y +
                                                novaDirecao.y)][(
                         int)(jogo->professores[i].pos.x + novaDirecao.x)] ==
                         '#' ||
                     jogo->labirintoAtual[(int)(jogo->professores[i].pos.y +
                                                novaDirecao.y)][(
                         int)(jogo->professores[i].pos.x + novaDirecao.x)] ==
                         '.') &&
                    k < 10);

                // Atualiza a direção do professor
                jogo->professores[i].dpos = novaDirecao;

            } else {
                // Caso contrário

                // Atualiza a posição do professor
                jogo->professores[i].pos = posPretendida;

                // Verifica se o professor encontrou o jogador
                if (abs(jogo->jogador.pos.x - jogo->professores[i].pos.x) <=
                        1 &&
                    abs(jogo->jogador.pos.y - jogo->professores[i].pos.y) <=
                        1) {
                    // Desativa o professor
                    jogo->professores[i].respondido = 1;

                    // Define a origem da pergunta
                    jogo->origemPergunta = 'P';
                    jogo->origemPerguntaPos = jogo->professores[i].pos;

                    // Muda a cena
                    mudarCena(jogo, CENA_PERGUNTA);
                }
            }
        }
    }
}

// Carrega os recursos do jogo
int carregarRecursos(JOGO *jogo) {
    int sucesso = 1;
    // Carrega as texturas
    sucesso *= carregarTexturas(&(jogo->texturas));

    // Carrega os sons
    carregarSons(&(jogo->sons));

    // Carrega as perguntas
    carregarPerguntas(&jogo->perguntas, &(jogo->qtdePerguntas));
    jogo->perguntaAtual = GetRandomValue(0, jogo->qtdePerguntas - 1);

    // Carrega a musica
    jogo->musica = LoadMusicStream(
        "resources/feed-the-machine-classic-arcade-game-116846.mp3");

    // Retorna se todos os recursos foram carregados com sucesso e se a
    // quantidade de perguntas é maior que 0
    return sucesso == 1 && jogo->qtdePerguntas > 0;
}

void descarregarRecursos(JOGO *jogo) {
    TEXTURESLIST *tl = &jogo->texturas;
    SOUNDLIST *sl = &jogo->sons;
    Texture2D *t;
    Sound *s;
    int i;

    // Quantidade de elementos de cada lista, baseado nos tamanhos
    int qtdeT = sizeof(TEXTURESLIST) / sizeof(Texture2D);
    int qtdeS = sizeof(SOUNDLIST) / sizeof(Sound);

    for (i = 0; i < qtdeT; i++) {
        t = ((Texture2D *)tl) + i;  // Ponteiro para o elemento i da lista
        UnloadTexture(*t);
    }

    for (i = 0; i < qtdeS; i++) {
        s = ((Sound *)sl) + i;  // Ponteiro para o elemento i da lista
        UnloadSound(*s);
    }

}

// Carrega as texturas
int carregarTexturas(TEXTURESLIST *texturas) {
    int sucesso = 1;
    sucesso *= carregaTextura(&texturas->colega, "./resources/colega.png");
    sucesso *= carregaTextura(&texturas->credito, "./resources/credito.png");
    sucesso *= carregaTextura(&texturas->livre, "./resources/livre.png");
    sucesso *=
        carregaTextura(&texturas->bombaItem, "./resources/bomba-item.png");
    sucesso *=
        carregaTextura(&texturas->bombaAtiva, "./resources/bomba-ativa.png");
    sucesso *= carregaTextura(&texturas->parede, "./resources/parede.png");
    sucesso *=
        carregaTextura(&texturas->professor, "./resources/professor.png");
    sucesso *= carregaTextura(&texturas->relogio, "./resources/relogio.png");
    sucesso *= carregaTextura(&texturas->saida, "./resources/saida.png");
    sucesso *= carregaTextura(&texturas->vida, "./resources/vida.png");
    sucesso *= carregaTextura(&texturas->jogador, "./resources/jogador.png");
    sucesso *= carregaTextura(&texturas->explosao, "./resources/explosao.png");
    sucesso *= carregaTextura(&texturas->disquete, "./resources/disquete.png");
    return sucesso;
}

// Carrega os sons
int carregarSons(SOUNDLIST *sons) {
    sons->select = LoadSound("resources/select.wav");
    sons->pickup = LoadSound("resources/pickup.wav");
    sons->hit = LoadSound("resources/hit.wav");
    sons->powerup = LoadSound("resources/powerup.wav");
    sons->select_m = LoadSound("resources/select_m.wav");
    sons->blocked = LoadSound("resources/blocked.wav");
    sons->timesUp = LoadSound("resources/timesup.wav");
    sons->gameOver = LoadSound("resources/gameover.wav");
    sons->correctAnswer = LoadSound("resources/correctanswer.wav");
    sons->wrongAnswer = LoadSound("resources/wronganswer.wav");
    sons->levelUp = LoadSound("resources/levelup.wav");
    sons->explosion = LoadSound("resources/explosion.wav");
}

// Inicializa um novo labirinto
void iniciarLabirinto(JOGO *jogo) {
    carregarLabirinto(jogo);
    popularLabirinto(jogo);

    // Inica o jogador
    jogo->jogador.pos = obterInicioLabirinto(jogo->labirintoAtual);
    jogo->jogador.creditosMapa = 0;
    jogo->jogador.tempoMapa = 0;
    jogo->tempoMaximo = 300;

    mudarCena(jogo, CENA_LABIRINTO);
}

// Movimenta o jogador no mapa
void atualizarJogador(JOGO *jogo) {
    Vector2 dpos = {0.0, 0.0}, proximaPos;

    // Se o jogador perdeu todas as vidas, muda para a cena de fim
    if (jogo->jogador.vidas <= 0) {
        salvarJogadorComoColega(jogo->jogador, jogo->nivel);
        // Muda para cena de fim de jogo
        mudarCena(jogo, CENA_FIM_JOGO);
    }

    // Movimenta o jogador
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        dpos.y = -1.0;
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        dpos.y = +1.0;
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        dpos.x = -1.0;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        dpos.x = +1.0;

    proximaPos = vectorAdd(jogo->jogador.pos, dpos);

    // Verifica se o jogador pode se mover para a proxima posição
    if (!vectorEqual(proximaPos, jogo->jogador.pos) &&
        jogo->labirintoAtual[(int)proximaPos.y][(int)proximaPos.x] != '#' &&
        jogo->labirintoAtual[(int)proximaPos.y][(int)proximaPos.x] != '.' &&
        menorIgual(0, proximaPos.x, TAM_LAB - 1) &&
        menorIgual(0, proximaPos.y, TAM_LAB - 1)) {
        // Move o jogador
        jogo->jogador.pos = proximaPos;
        jogo->jogador.dpos = dpos;

        // Verifica se o jogador pegou algum item
        switch (jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                                    [(int)jogo->jogador.pos.x]) {
            // Se pegou um crédito
            case 'C':
                PlaySound(jogo->sons.powerup);
                jogo->jogador.creditosMapa++;
                jogo->jogador.creditosTotal++;
                jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                                    [(int)jogo->jogador.pos.x] = ' ';
                break;
            // Se está na saída do labirinto
            case 'S':
                if (jogo->jogador.creditosMapa >= jogo->creditosNecessarios) {
                    jogo->nivel++;
                    PlaySound(jogo->sons.levelUp);

                    jogo->corParede =
                        (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                                GetRandomValue(0, 255), 255};
                    iniciarLabirinto(jogo);
                } else {
                    // Só toca o som se o jogador estiver se movendo
                    if (!vectorEqual(dpos, (Vector2){0.0, 0.0}))
                        PlaySound(jogo->sons.blocked);
                }
                break;
            // Se pegou um relógio
            case 'R':
                jogo->tempoMaximo += 30;
                PlaySound(jogo->sons.pickup);
                jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                                    [(int)jogo->jogador.pos.x] = ' ';
                break;
            // Se pegou uma vida
            case 'V':
                jogo->jogador.vidas += 1;
                PlaySound(jogo->sons.pickup);
                jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                                    [(int)jogo->jogador.pos.x] = ' ';
                break;
                // Se pegou uma bomba
            case 'B':
                jogo->jogador.bombas += 1;
                PlaySound(jogo->sons.pickup);
                jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                                    [(int)jogo->jogador.pos.x] = ' ';
                break;
        }

        // Verifica se o jogador enctrou um colega
        for (int i = 0; i < jogo->qtdeColegas; i++) {
            if (abs(jogo->jogador.pos.x - jogo->colegas[i].pos.x) <= 1 &&
                abs(jogo->jogador.pos.y - jogo->colegas[i].pos.y) <= 1 &&
                !jogo->colegas[i].respondido) {
                // Desativa o colega
                jogo->colegas[i].respondido = 1;
                // Remove o colega do arquivo
                removerColegaDoArquivo(jogo->colegas[i]);

                // Define a origem da pergunta
                jogo->origemPergunta = 'c';
                jogo->origemPerguntaPos = jogo->colegas[i].pos;

                // Muda a cena
                mudarCena(jogo, CENA_PERGUNTA);
            }
        }
    }
};

// Verifica se o professor possui linha de visão com o jogador, sem parades
// no meio, limitado vertical e horiziontalmente
int irEmDirecaoAojogador(PROFESSOR *p, JOGADOR *a, char lab[TAM_LAB][TAM_LAB]) {
    int i, possuiVisao = 1;
    Vector2 dpos = {0.0, 0.0};
    Vector2 vpos = p->pos;

    if (p->pos.x == a->pos.x)
        dpos.y = p->pos.y - a->pos.y > 0 ? -1 : 1;
    else if (p->pos.y == a->pos.y)
        dpos.x = p->pos.x - a->pos.y > 0 ? -1 : 1;

    if (dpos.x != 0 || dpos.y != 0) {
        while (possuiVisao && (vpos.x != a->pos.x || vpos.y != a->pos.y)) {
            vpos = vectorAdd(vpos, dpos);
            if (lab[(int)vpos.y][(int)vpos.x] == '#' ||
                lab[(int)vpos.y][(int)vpos.x] == '.')
                possuiVisao = 0;
        }
    } else
        possuiVisao = 0;

    if (possuiVisao) p->dpos = dpos;

    return possuiVisao;
}

int colocarBomba(Vector2 pos, BOMBA bombasAtivas[], int *qtdeBombas) {
    int i, colocou = 0;

    // Se tiver espaço para colocar mais bombas
    if (*qtdeBombas < MAX_BOMBAS_ATIVAS - 1) {
        // Desloca as bombas para a direita
        for (i = *qtdeBombas; i >= 0; i--)
            bombasAtivas[i + 1] = bombasAtivas[i];

        // Incrementa a quantidade de bombas
        *qtdeBombas += 1;

        // Insiere uma nova bomba na primeira posição
        bombasAtivas[0] = (BOMBA){pos, BOMBA_TEMPO_EXPLOSAO};
        colocou = 1;
    }

    return colocou;
}

void atualizarBombas(JOGO *jogo) {
    int i;
    // Itera pro todas as bombas ativas
    for (i = 0; i < jogo->qtdeBombasAtivas; i++) {
        // Se o momento de explosão da bomba for maior que o tempo de jogo
        // atual, a bomba explode
        if (jogo->bombasAtivas[i].explodeEm < 0) {
            explodirBomba(jogo, jogo->bombasAtivas[i]);
        } else {
            // Decrementa o tempo de explosão da bomba
            jogo->bombasAtivas[i].explodeEm -= GetFrameTime();
        }
    }
}

void explodirBomba(JOGO *jogo, BOMBA bomba) {
    int i, j = 0;

    // Remove a bomba da lista de bombas ativas
    for (i = 0; i < jogo->qtdeBombasAtivas; i++) {
        if (vectorEqual(jogo->bombasAtivas[i].pos, bomba.pos)) {
            // Desloca as bombas para a esquerda
            for (j = i; j < jogo->qtdeBombasAtivas; j++)
                jogo->bombasAtivas[j] = jogo->bombasAtivas[j + 1];

            // Decrementa a quantidade de bombas
            jogo->qtdeBombasAtivas--;
        }
    }

    // Remove os blocos ao redor da bomba
    for (i = -BOMBA_RAIO_EXPLOSAO; i <= BOMBA_RAIO_EXPLOSAO; i++) {
        for (j = -BOMBA_RAIO_EXPLOSAO; j <= BOMBA_RAIO_EXPLOSAO; j++) {
            if (menorIgual(0, (int)bomba.pos.y + i, TAM_LAB) &&
                menorIgual(0, (int)bomba.pos.x + j, TAM_LAB))

                // Se o bloco não for uma saida, inicio ou uma parede
                // permanente, remove ele
                if (jogo->labirintoAtual[(int)bomba.pos.y + i]
                                        [(int)bomba.pos.x + j] != 'S' &&
                    jogo->labirintoAtual[(int)bomba.pos.y + i]
                                        [(int)bomba.pos.x + j] != '.' &&
                    jogo->labirintoAtual[(int)bomba.pos.y + i]
                                        [(int)bomba.pos.x + j] != 'I')
                    jogo->labirintoAtual[(int)bomba.pos.y + i]
                                        [(int)bomba.pos.x + j] = ' ';
        }
    }

    // Verifica se o jogador está próximo da bomba
    if (abs(jogo->jogador.pos.x - bomba.pos.x) <= BOMBA_RAIO_EXPLOSAO &&
        abs(jogo->jogador.pos.y - bomba.pos.y) <= BOMBA_RAIO_EXPLOSAO) {
        // Remove uma vida do jogador e reposiciona para o inicio do
        // labirinto
        jogo->jogador.vidas--;
        jogo->jogador.pos = obterInicioLabirinto(jogo->labirintoAtual);
        PlaySound(jogo->sons.hit);
    }

    // Verifica se um aluno está próximo da bomba
    for (i = 0; i < jogo->qtdeColegas; i++) {
        if (abs(jogo->colegas[i].pos.x - bomba.pos.x) <= BOMBA_RAIO_EXPLOSAO &&
            abs(jogo->colegas[i].pos.y - bomba.pos.y) <= BOMBA_RAIO_EXPLOSAO) {
            // Desativa o colega
            jogo->colegas[i].respondido = 1;
        }
    }

    // Verifica se o professor está próximo da bomba
    for (i = 0; i < jogo->qtdeProfessores; i++) {
        if (abs(jogo->professores[i].pos.x - bomba.pos.x) <=
                BOMBA_RAIO_EXPLOSAO &&
            abs(jogo->professores[i].pos.y - bomba.pos.y) <=
                BOMBA_RAIO_EXPLOSAO) {
            // Desativa o professor
            jogo->professores[i].respondido = 1;
        }
    }

    PlaySound(jogo->sons.explosion);

    BeginDrawing();
    BeginMode2D(jogo->camera);
    DrawTextureAutoScale(
        jogo->texturas.explosao,
        (Vector2){(bomba.pos.x - BOMBA_RAIO_EXPLOSAO) * BLOCK_SIZE,
                  (bomba.pos.y - BOMBA_RAIO_EXPLOSAO) * BLOCK_SIZE},
        0, (2 * BOMBA_RAIO_EXPLOSAO + 1) * BLOCK_SIZE, WHITE);
    EndMode2D();
    EndDrawing();

    // Explode as bombas ao redor da bomba que explodiu, função recursiva :)
    for (i = 0; i < jogo->qtdeBombasAtivas; i++) {
        if (abs(jogo->bombasAtivas[i].pos.x - bomba.pos.x) <=
                BOMBA_RAIO_EXPLOSAO &&
            abs(jogo->bombasAtivas[i].pos.y - bomba.pos.y) <=
                BOMBA_RAIO_EXPLOSAO) {
            explodirBomba(jogo, jogo->bombasAtivas[i]);
        }
    }
}
