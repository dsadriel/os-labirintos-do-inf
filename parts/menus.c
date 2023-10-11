#include "menus.h"
/*
Todas as funções que desenham menu estão aqui
*/

// Loop do jogo, aqui acontecem a lógica e atualizações do jogo
void menuLabirinto(JOGO *jogo) {
    // Se a tecla ESC for pressionada ou a janela perder o foco, pausa o jogo
    if (IsKeyPressed(KEY_ESCAPE) || !IsWindowFocused())
        mudarCena(jogo, CENA_MENU_PAUSE);

    // Se a musica estiver pausada, resume
    if (!IsMusicStreamPlaying(jogo->musica)) ResumeMusicStream(jogo->musica);

    // Atualiza o tempo do jogador
    atualizarTempo(jogo);

    // Atualiza as bombas
    atualizarBombas(jogo);

    // Se a quantidade de créditos disponíveis no mapa for menor que a
    // quantidade necessária para passar o labirinto, insere mais créditos no
    // mapa
    if (contarCreditosDiponiveis(jogo->professores, jogo->qtdeProfessores,
                                 jogo->labirintoAtual) <
        jogo->creditosNecessarios - jogo->jogador.creditosMapa)
        for (int i = 0; i < 5; i++)
            preencherVazioAleatorioCom(jogo->labirintoAtual, 'C');

    // Verifica se o jogador tentou colocar uma bomba
    if (IsKeyPressed(KEY_SPACE)) {
        // Verifica se o jogador tem bombas disponíveis
        if (jogo->jogador.bombas > 0) {
            jogo->jogador.bombas--;

            colocarBomba(jogo->jogador.pos, jogo->bombasAtivas,
                         &jogo->qtdeBombasAtivas);
        } else
            PlaySound(jogo->sons.blocked);
    }

    // Verifica onde se encontra o jogador e move a origem do camera
    switch (obterQuadrante(jogo->jogador.pos, TAM_LAB, TAM_LAB)) {
        case 1:
            jogo->camera.offset = (Vector2){BLOCK_SIZE * -50, 0};
            break;
        case 2:
            jogo->camera.offset = (Vector2){0, 0};
            break;
        case 3:
            jogo->camera.offset = (Vector2){0, BLOCK_SIZE * -50};
            break;
        case 4:
            jogo->camera.offset = (Vector2){BLOCK_SIZE * -50, BLOCK_SIZE * -50};
            break;
    }

    // Somente atualiza os jogadores e professores a cada 15 frames
    if ((int)(GetTime() * GetFPS()) % 2 == 1) {
        // Move o jogador
        atualizarJogador(jogo);
        // Atualiza os professores
        atualizarProfessores(jogo);
    }

    // Atualiza a pontuação
    jogo->jogador.pontuacao =
        (10.0 * MAX_CREDITOS_LAB * jogo->nivel) +
        ((float)jogo->jogador.creditosTotal / jogo->jogador.tempoTotal);
    /*  10 * max * L + C/T , onde: max é um número maior que
    a quantidade de créditos necessários para passar qualquer labirinto, L é
    a quantidade de labirintos terminados, C o total de créditos acumulados
    ao longo do jogo e T o tempo total consumido durante o jogo.
    */

    // Desenha o labirinto
    BeginDrawing();
    ClearBackground(BLACK);

    desenharLabirinto(jogo);

    desenharBarraLateral(jogo);

    // Se o jogador estiver em uma célula 'S' e não tiver créditos suficientes
    if (jogo->labirintoAtual[(int)jogo->jogador.pos.y]
                            [(int)jogo->jogador.pos.x] == 'S' &&
        jogo->jogador.creditosMapa < jogo->creditosNecessarios) {
        DrawTextRec("Você não tem créditos suficientes para sair do labirinto!",
                    (Rectangle){(BLOCK_SIZE)*5, GetScreenHeight() * 0.8,
                                BLOCK_SIZE * 40, 0},
                    BLOCK_SIZE * 2, RED);
    }

    if (strcmp(jogo->jogador.nome, DEBUG_USERNAME) == 0)
        DrawTextRec(
            "DEBUG MODE (CTRL+)      ENTER: Nivel++     F1: Bomba++      F2: "
            "Creditos++      F3: Vidas--      APAGAR: TempoMapa=0     F5: "
            "PERGUNTA",
            (Rectangle){0, 0, BLOCK_SIZE * 50, 0}, 20, RED);

    EndDrawing();

    // DEBUG
    if (strcmp(jogo->jogador.nome, DEBUG_USERNAME) == 0) {
        // Avança um nível
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ENTER)) {
            jogo->nivel++;
            jogo->corParede =
                (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                        GetRandomValue(0, 255), 255};
            iniciarLabirinto(jogo);
        };

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F1)) {
            jogo->jogador.bombas++;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F2)) {
            jogo->jogador.creditosMapa++;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F3)) {
            jogo->jogador.vidas--;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_BACKSPACE)) {
            jogo->tempoRestante = 0;
            jogo->jogador.tempoMapa = 10000000000000;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F5)) {
            jogo->origemPergunta = 'P';
            jogo->origemPerguntaPos = jogo->jogador.pos;
            mudarCena(jogo, CENA_PERGUNTA);
        }
    }
}

void menuSalvarJogo(JOGO *jogo) {
    // Se a mensagem do jogo não for relacionado a salvar, salva o jogo
    if (strcmp(jogo->mensagem, "Jogo salvo com sucesso!") != 0 &&
        strcmp(jogo->mensagem, "Erro ao salvar o jogo!") != 0) {
        if (salvarJogo(jogo) == 1) {
            strcpy(jogo->mensagem, "Jogo salvo com sucesso!");
        } else {
            strcpy(jogo->mensagem, "Erro ao salvar o jogo!");
        };
    }

    if (IsKeyPressed(KEY_ENTER)) {
        jogo->mensagem[0] = '\0';
        mudarCena(jogo, CENA_LABIRINTO);
    } else if (IsKeyPressed(KEY_ESCAPE)) {
        jogo->mensagem[0] = '\0';
        mudarCena(jogo, CENA_MENU_PRINCIPAL);
    }

    BeginDrawing();
    // Desenha o fundo do menu
    desenharLabirinto(jogo);
    desenharBarraLateral(jogo);

    // Desenhar o menu
    DrawRectangle(BLOCK_SIZE * 4, BLOCK_SIZE * 9, BLOCK_SIZE * 42,
                  BLOCK_SIZE * 32, jogo->mensagem[0] == 'E' ? RED : GREEN);
    DrawRectangle(BLOCK_SIZE * 5, BLOCK_SIZE * 10, BLOCK_SIZE * 40,
                  BLOCK_SIZE * 30, BLACK);

    DrawTextureAutoScale(jogo->texturas.disquete,
                         (Vector2){BLOCK_SIZE * 20, BLOCK_SIZE * 15}, 0.0f,
                         BLOCK_SIZE * 10, WHITE);

    DrawTextCenter(jogo->mensagem, BLOCK_SIZE * 25, BLOCK_SIZE * 27, 40,
                   jogo->mensagem[0] == 'E' ? RED : GREEN);
    DrawTextCenter("<pressione ENTER continuar o jogo>", BLOCK_SIZE * 25,
                   BLOCK_SIZE * 38 - 25, 20, GRAY);
    DrawTextCenter(" <prescione ESC para voltar ao menu principal>",
                   BLOCK_SIZE * 25, BLOCK_SIZE * 38, 20, GRAY);

    EndDrawing();
}

// Desenha o menu principal
void menuPrincipal(JOGO *jogo) {
    // Navegar entre as opções
    alterarItemSelecionado(&jogo->itemSelecionado, 5);

    if (!IsMusicStreamPlaying(jogo->musica)) ResumeMusicStream(jogo->musica);

    // Se pressionar enter ou espaço, altere o menu
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        switch (jogo->itemSelecionado) {
            case 0:
                mudarCena(jogo, CENA_MENU_NOVOJOGO1);
                break;
            case 1:
                jogo->qtdeSaves = carregarSaves(jogo->saves);
                mudarCena(jogo, CENA_MENU_CARREGAR);
                break;
            case 2:
                mudarCena(jogo, CENA_MENU_CLASSIFICACAO);
                break;
            case 3:
                mudarCena(jogo, CENA_MENU_INFORMACOES);
                break;
            case 4:
                mudarCena(jogo, CENA_SAIR);
                break;
        }
    }

    // Desenhar o menu
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                   GetScreenHeight() / 3, 60, WHITE);

    // Desenha as opções
    char t[5][MAX_OPTLEN] = {"Novo jogo", "Carregar jogo", "Exibir ganhadores",
                             "Informações", "Sair"};
    DrawTextRecList(
        t, 5,
        (Rectangle){100, GetScreenHeight() - 275, GetScreenWidth() - 100, 0},
        45, RED, WHITE, jogo->itemSelecionado);

    EndDrawing();
}
void menuConfirmarSaida(JOGO *jogo) {
    // Navegar entre as opções
    alterarItemSelecionado(&jogo->itemSelecionado, 2);

    // Se pressionar enter ou espaço, altere o menu
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        switch (jogo->itemSelecionado) {
            case 0:
                jogo->sair = 1;
                break;
            case 1:
                mudarCena(jogo, -1);
                break;
        }
    }

    // Desenhar o menu
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextCenter("Deseja realmente sair?", GetScreenWidth() / 2,
                   GetScreenHeight() / 3, 60, WHITE);

    // Desenha as opções
    char t[2][MAX_OPTLEN] = {"Sim", "Não"};
    DrawTextRecList(
        t, 2,
        (Rectangle){100, GetScreenHeight() - 275, GetScreenWidth() - 100, 0},
        45, RED, WHITE, jogo->itemSelecionado);
    EndDrawing();
}

// Desenha o menu de novo jogo
void menuNovojogo(JOGO *jogo) {
    char blink;
    int key, len, i;

    // Limpa os dados do o jogador
    if (jogo->jogador.tempoTotal != 0) {
        jogo->jogador = (JOGADOR){0};
    }

    if (jogo->cenaAtual == CENA_MENU_NOVOJOGO1) {
        // Se for apertado ESC, volta para o menu principal
        if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, CENA_MENU_PRINCIPAL);

        len = strlen(jogo->jogador.nome);

        // Se for apertado enter e o nome não for vazio
        if (IsKeyPressed(KEY_ENTER) && len > 0) {
            mudarCena(jogo, CENA_MENU_NOVOJOGO2);
        }

        // Lê o nome do jogador
        key = GetCharPressed();

        // Se o carácter pressionado for válido e o nome não tiver atingido
        // o máximo
        if ((key >= 32) && (key <= 125) && (len < MAX_NOME_JOGADOR)) {
            jogo->jogador.nome[len] = (char)key;
            jogo->jogador.nome[len + 1] = '\0';
            PlaySound(jogo->sons.select_m);
        }

        // Se for apertado pra apagar
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (len > 0) jogo->jogador.nome[len - 1] = '\0';
            PlaySound(jogo->sons.select_m);
        }

        //"_" piscando
        blink = (int)GetTime() % 2 == 0 && len < MAX_NOME_JOGADOR ? '_' : ' ';

        // Desenha o menu
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                       GetScreenHeight() / 5, 60, WHITE);
        DrawTextCenter("Novo jogo", GetScreenWidth() / 2,
                       GetScreenHeight() / 5 + 70, 30, GRAY);
        DrawTextCenter("Qual seu nome?", GetScreenWidth() / 2,
                       GetScreenHeight() / 2 - 50, 25, WHITE);
        DrawTextCenter(TextFormat("%s%c", jogo->jogador.nome, blink),
                       GetScreenWidth() / 2, GetScreenHeight() / 2, 40, WHITE);

        DrawTextCenter("<pressione ENTER para confirmar>", GetScreenWidth() / 2,
                       GetScreenHeight() - 50, 20, GRAY);
        EndDrawing();

    } else {  // Seletor de dificuldade
        if (IsKeyPressed(KEY_ENTER)) {
            // Inicia o jogo

            jogo->dificuldade = jogo->itemSelecionado + 1;
            jogo->nivel = 1;
            jogo->jogador.vidas = (5 - jogo->dificuldade) * 2;
            jogo->jogador.bombas = 3;

            // Remove as bombas ativas
            for (i = 0; i < MAX_BOMBAS_ATIVAS; i++)
                jogo->bombasAtivas[i] = (BOMBA){0};
            jogo->qtdeBombasAtivas = 0;

            // Inicia o labirinto
            iniciarLabirinto(jogo);
        }

        if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, -1);

        alterarItemSelecionado(&jogo->itemSelecionado, 4);

        // Desenha o seletor de dificuldade
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                       GetScreenHeight() / 5, 60, WHITE);
        DrawTextCenter("Novo jogo", GetScreenWidth() / 2,
                       GetScreenHeight() / 5 + 70, 30, GRAY);
        DrawTextCenter("Selecionar dificuldade", GetScreenWidth() / 2,
                       GetScreenHeight() / 2 - 50, 25, WHITE);

        char opt[4][MAX_OPTLEN] = {"Fácil", "Médio", "Difícil", "Impossível"};

        DrawTextCenterList(opt, 4, GetScreenWidth() / 2,
                           GetScreenHeight() / 2 + 40 * 2, 40, RED, WHITE,
                           jogo->itemSelecionado);

        DrawTextCenter("<pressione ENTER para confirmar>", GetScreenWidth() / 2,
                       GetScreenHeight() - 50, 20, GRAY);
        EndDrawing();
    }
}

void menuPause(JOGO *jogo) {
    int opcoesMenu, font_size, dx;

    // Volta ao jogo
    if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, CENA_LABIRINTO);

    // Pausa a música
    if (IsMusicStreamPlaying(jogo->musica)) PauseMusicStream(jogo->musica);

    // Navegar entre as opções
    alterarItemSelecionado(&jogo->itemSelecionado, 6);

    // Se pressionar enter ou espaço, altera o menu
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        switch (jogo->itemSelecionado) {
            case 0:
                mudarCena(jogo, CENA_LABIRINTO);
                break;
            case 1:
                iniciarLabirinto(jogo);
                break;
            case 2:
                mudarCena(jogo, CENA_MENU_SALVAR);
                break;
            case 3:
                mudarCena(jogo, CENA_MENU_NOVOJOGO1);
                break;
            case 4:
                mudarCena(jogo, MENU_TUTORIAL);
                break;
            case 5:
                mudarCena(jogo, CENA_SAIR);
                break;
        }
    }

    BeginDrawing();
    // Desenhar o fundo
    desenharLabirinto(jogo);
    desenharBarraLateral(jogo);

    // Desenhar o menu
    DrawRectangle(BLOCK_SIZE * 4, BLOCK_SIZE * 4, BLOCK_SIZE * 42,
                  BLOCK_SIZE * 42, DARKGRAY);
    DrawRectangle(BLOCK_SIZE * 5, BLOCK_SIZE * 5, BLOCK_SIZE * 40,
                  BLOCK_SIZE * 40, BLACK);

    DrawTextCenter("[jogo pausado]", BLOCK_SIZE * 25, GetScreenHeight() / 4, 45,
                   GRAY);

    // Desenha as opções;
    DrawTextCenterList(
        (char[6][MAX_OPTLEN]){"Retomar o jogo", "Reiniciar labirinto atual",
                              "Salvar jogo", "Novo jogo", "Abrir tutorial",
                              "Sair do jogo"},
        6, BLOCK_SIZE * 25, GetScreenHeight() / 2, 30, RED, WHITE,
        jogo->itemSelecionado);

    DrawTextCenter("<pressione ESC para voltar ao jogo>", BLOCK_SIZE * 25,
                   BLOCK_SIZE * 40, 15, GRAY);
    EndDrawing();
}

void menuPergunta(JOGO *jogo) {
    int font_size, dx, i;
    Rectangle boxTexto;
    Vector2 ultimoDesenho = {0, 0};
    char recompensa;

    // Atualiza o tempo
    atualizarTempo(jogo);

    // Escolhe uma pergunta aleatória
    i = 0;
    while (jogo->perguntas[jogo->perguntaAtual].ja_respondida == 1 &&
           i < jogo->qtdePerguntas) {
        jogo->perguntaAtual = GetRandomValue(0, jogo->qtdePerguntas - 1);

        i++;
    }

    // Se não há mais perguntas, repetir a última aleatória
    if (i == jogo->qtdePerguntas)
        jogo->perguntas[jogo->perguntaAtual].ja_respondida = 0;

    PERGUNTA p = jogo->perguntas[jogo->perguntaAtual];

    // Navegar entre as opções
    alterarItemSelecionado(&jogo->itemSelecionado, p.n_alternativas);

    // Ao pressionar enter
    if (IsKeyPressed(KEY_ENTER)) {
        // Marca a pergunta como respondida
        jogo->perguntas[jogo->perguntaAtual].ja_respondida = 1;

        // Se a resposta está correta
        if (jogo->perguntas[jogo->perguntaAtual].resposta_correta ==
            (jogo->itemSelecionado + 1)) {
            // Se for um professor, deixa um crédito
            if (jogo->origemPergunta == 'P') {
                recompensa = 'C';
            } else {
                switch (GetRandomValue(0, 2)) {
                    case 0:
                        recompensa = 'V';
                        break;
                    case 1:
                        recompensa = 'R';
                        break;
                    case 2:
                        recompensa = 'B';
                        break;
                }
            }

            // Adiciona a recompensa no labirinto
            jogo->labirintoAtual[(int)jogo->origemPerguntaPos.y]
                                [(int)jogo->origemPerguntaPos.x] = recompensa;

            PlaySound(jogo->sons.correctAnswer);

        } else {  // Se a resposta está errada

            // Reproduz o som de resposta errada
            PlaySound(jogo->sons.wrongAnswer);

            // Se for um professor, perde uma vida
            jogo->jogador.vidas--;
        }

        // Muda para a cena do labirinto
        mudarCena(jogo, CENA_LABIRINTO);

    } else {
        BeginDrawing();

        // Desenha o fundo/labirinto
        desenharLabirinto(jogo);
        desenharBarraLateral(jogo);

        // Desenhar o menu
        DrawRectangle(BLOCK_SIZE * 4, BLOCK_SIZE * 4, BLOCK_SIZE * 42,
                      BLOCK_SIZE * 42, DARKGRAY);
        DrawRectangle(BLOCK_SIZE * 5, BLOCK_SIZE * 5, BLOCK_SIZE * 40,
                      BLOCK_SIZE * 40, BLACK);

        // Escreve a pergunta na tela
        boxTexto =
            (Rectangle){BLOCK_SIZE * 6, BLOCK_SIZE * 6, BLOCK_SIZE * 38, 0};
        ultimoDesenho =
            DrawTextRec(TextFormat("%s", p.pergunta), boxTexto, 30, GRAY);

        // Desenha o personagem
        desenharSprite(vectorMultEscalar((Vector2){37, 43}, BLOCK_SIZE),
                       (Vector2){0, 0}, 14,
                       jogo->origemPergunta == 'P' ? jogo->texturas.professor
                                                   : jogo->texturas.colega,
                       WHITE);

        // Escreve as opções
        boxTexto = (Rectangle){BLOCK_SIZE * 6, ultimoDesenho.y + 30 * 2,
                               BLOCK_SIZE * 38};
        DrawTextRecList(p.alternativas, p.n_alternativas, boxTexto, 30, RED,
                        WHITE, jogo->itemSelecionado);

        EndDrawing();
    }
}

// Menu de carregamento de jogos
void menuCarregarJogo(JOGO *jogo) {
    int i;

    if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, -1);
    alterarItemSelecionado(&jogo->itemSelecionado, jogo->qtdeSaves);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                   GetScreenHeight() / 5, 60, WHITE);
    DrawTextCenter("Carregar jogo", GetScreenWidth() / 2,
                   GetScreenHeight() / 5 + 70, 30, GRAY);
    DrawTextCenter("Selecione o jogo a ser carregado", GetScreenWidth() / 2,
                   GetScreenHeight() / 5 + 105, 25, GRAY);

    DrawTextCenter("<pressione ESC para voltar ao menu principal>",
                   GetScreenWidth() / 2, GetScreenHeight() - 50, 20, GRAY);

    if (jogo->qtdeSaves > 0) {
        DrawTextCenter(
            TextFormat("%d de %d", jogo->itemSelecionado + 1, jogo->qtdeSaves),
            GetScreenWidth() / 2, GetScreenHeight() / 5 + 140, 25, WHITE);

        DrawTextCenter("Use as teclas para alternar e enter para carregar",
                       GetScreenWidth() / 2, GetScreenHeight() / 2 + 200, 20,
                       GRAY);

        i = jogo->itemSelecionado;

        DrawText(jogo->saves[i].jogador.nome, 100, GetScreenHeight() / 2, 40,
                 WHITE);

        DrawText(TextFormat("Nível: %d", jogo->saves[i].nivel), 100,
                 GetScreenHeight() / 2 + 45, 30, GRAY);
        DrawText(TextFormat("Dificuldade: %d", jogo->saves[i].dificuldade), 300,
                 GetScreenHeight() / 2 + 45, 30, GRAY);
        DrawText(TextFormat("Vidas: %d", jogo->saves[i].jogador.vidas), 100,
                 GetScreenHeight() / 2 + 75, 30, GRAY);
        DrawText(
            TextFormat("Créditos: %d", jogo->saves[i].jogador.creditosTotal),
            300, GetScreenHeight() / 2 + 75, 30, GRAY);

        DrawText(TextFormat("Tempo total: %dm%ds",
                            (int)jogo->saves[i].jogador.tempoTotal / 60,
                            (int)jogo->saves[i].jogador.tempoTotal % 60),
                 100, GetScreenHeight() / 2 + 105, 30, GRAY);

        if (IsKeyPressed(KEY_ENTER)) {
            carregarJogo(jogo, jogo->saves[jogo->itemSelecionado]);
            mudarCena(jogo, CENA_LABIRINTO);
        }
    } else {
        DrawTextCenter("Nenhum jogo salvo :(", GetScreenWidth() / 2,
                       GetScreenHeight() / 2, 40, WHITE);
    }

    EndDrawing();
}

// Desenha o menu sobre, com informações sobre o jogo e desenvolvedores
void menuInformacoes(JOGO *jogo) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        mudarCena(jogo, CENA_MENU_PRINCIPAL);
    }

    if (IsKeyPressed(KEY_T)) {
        mudarCena(jogo, MENU_TUTORIAL);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                   GetScreenHeight() / 5, 60, WHITE);
    DrawTextCenter("Informações", GetScreenWidth() / 2,
                   GetScreenHeight() / 5 + 70, 30, GRAY);

    DrawTextCenter("Desenvolvedores:", GetScreenWidth() / 2,
                   GetScreenHeight() / 3, 30, WHITE);
    DrawTextCenter("Adriel de Souza", GetScreenWidth() / 2,
                   GetScreenHeight() / 3 + 40, 32, COLOR_GOLD);
    DrawTextCenter("e João Pedro Müller Alvarenga", GetScreenWidth() / 2,
                   GetScreenHeight() / 3 + 75, 30, COLOR_GOLD);

    DrawTextCenter(
        "Professores: Dennis Giovani Balreira e Santiago Valdes Ravelo",
        GetScreenWidth() / 2, GetScreenHeight() / 3 + 115, 20, WHITE);

    DrawTextCenter(
        "Jogo desenvolvido como Trabalho Final do Algoritmos de "
        "Programação - 2023/1",
        GetScreenWidth() / 2, GetScreenHeight() / 3 + 140, 17, GRAY);

    DrawTextCenter("Créditos e agradecimentos:", GetScreenWidth() / 2,
                   GetScreenHeight() / 2 + 50, 30, WHITE);

    DrawTextRec(
        "Música por Dream-Protocol de Pixabay / Sons feitos com "
        "\"drpetter's "
        "sfxr tool\" e \"chioptone by sfbgames\" / \"Base Character "
        "Sprite\" "
        "por ZeoMaddox",
        (Rectangle){100, GetScreenHeight() / 2 + 100, GetScreenWidth() - 100,
                    GetScreenHeight() - 60},
        20, GRAY);

    DrawTextCenter("<pressione T para abrir o tutorial>", GetScreenWidth() / 2,
                   GetScreenHeight() - 60, 20, COLOR_GOLD);

    DrawTextCenter("<pressione ESC para voltar ao menu principal>",
                   GetScreenWidth() / 2, GetScreenHeight() - 30, 15, GRAY);

    EndDrawing();
}

// Desenha o menu de saida do jogo
void menuEspecialSaindo(char mensagem[]) {
    BeginDrawing();

    ClearBackground(BLACK);
    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2,
                   GetScreenHeight() / 5, 60, WHITE);
    DrawTextCenter("Saindo do jogo...", GetScreenWidth() / 2,
                   GetScreenHeight() / 5 + 70, 30, GRAY);

    if (strcmp(mensagem, "") == 0) {
        DrawTextCenter("Obrigado por jogar :)", GetScreenWidth() / 2,
                       GetScreenHeight() / 2, 40, WHITE);
    } else {
        DrawTextCenter(mensagem, GetScreenWidth() / 2, GetScreenHeight() / 2,
                       40, RED);
    }

    EndDrawing();
}

// Cena especial: acabou tempo
void menuAcabouTempo(JOGO *jogo) {
    static int executado = 0;

    // Se ainda não foi removido uma vida, remove
    if (!executado) {
        executado = 1;
        jogo->jogador.vidas--;
        PlaySound(jogo->sons.timesUp);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        iniciarLabirinto(jogo);

        executado = 0;
    }

    BeginDrawing();
    // Desenha o fundo do menu
    desenharLabirinto(jogo);
    desenharBarraLateral(jogo);

    // Desenhar o menu
    DrawRectangle(BLOCK_SIZE * 4, BLOCK_SIZE * 9, BLOCK_SIZE * 42,
                  BLOCK_SIZE * 32, RED);
    DrawRectangle(BLOCK_SIZE * 5, BLOCK_SIZE * 10, BLOCK_SIZE * 40,
                  BLOCK_SIZE * 30, BLACK);

    DrawTextureAutoScale(jogo->texturas.relogio,
                         (Vector2){BLOCK_SIZE * 20, BLOCK_SIZE * 12}, 0.0f,
                         BLOCK_SIZE * 10, WHITE);

    DrawTextCenter("Tempo esgotado!", BLOCK_SIZE * 25, BLOCK_SIZE * 25, 40,
                   RED);
    DrawTextCenter("Você não conseguiu chegar ao final há tempo...",
                   BLOCK_SIZE * 25, BLOCK_SIZE * 25 + 50, 20, WHITE);
    DrawTextCenter("<pressione ENTER para continuar>", BLOCK_SIZE * 25,
                   BLOCK_SIZE * 38, 20, GRAY);
    EndDrawing();
}

// Cena especial: fim de jogo
void menuFimDeJogo(JOGO *jogo) {
    static int executado = 0;
    static int fade = 0;

    // Pausa a música na cena de game over
    if (IsMusicStreamPlaying(jogo->musica)) PauseMusicStream(jogo->musica);

    // Se o último colega do vetor não for igual a posição do jogador,
    // adiciona o jogador na lista de colegas e salva a lista
    // PARA EVITAR LOOP, uma vez que a função é chamada diversas vezes

    if (fade < 250) {
        fade += 5;

        BeginDrawing();
        // Desenha o fundo do menu
        desenharLabirinto(jogo);
        desenharBarraLateral(jogo);
        // Tela preta de fundo
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      (Color){0, 0, 0, fade});
        EndDrawing();

    } else {
        if (!executado) {
            executado = 1;
            PlaySound(jogo->sons.gameOver);
        }

        if (IsKeyPressed(KEY_ENTER)) {
            // Só salva o ranking se a pontuação for maior que 500 e não for
            // jogador debug
            if (jogo->jogador.pontuacao > 500 &&
                strcpy(jogo->jogador.nome, DEBUG_USERNAME) != 0) {
                salvarClassificacao(jogo->jogador, jogo->dificuldade,
                                    &jogo->cenaAtual, jogo->mensagem);

                jogo->qtdeRanks = carregarClassificacao(jogo->ranking);
            }

            mudarCena(jogo, CENA_MENU_CLASSIFICACAO);
            // Resetar variaveis locais
            fade = executado = 0;
        } else if (IsKeyPressed(KEY_ESCAPE)) {
            mudarCena(jogo, CENA_MENU_CLASSIFICACAO);
            // Resetar variaveis locais
            fade = executado = 0;
        }

        BeginDrawing();
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        // Desenhar o menu
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);

        DrawRectangle(BLOCK_SIZE, BLOCK_SIZE, GetScreenWidth() - BLOCK_SIZE * 2,
                      GetScreenHeight() - BLOCK_SIZE * 2, BLACK);

        desenharSprite((Vector2){GetScreenWidth() / 2,
                                 GetScreenHeight() / 2 - BLOCK_SIZE * 3},
                       (Vector2){0, 0}, 11, jogo->texturas.colega, WHITE);

        DrawTextCenter("Você perdeu!", GetScreenWidth() / 2,
                       GetScreenHeight() / 2, 50, RED);
        DrawTextCenter("Acabaram sua vidas :(", GetScreenWidth() / 2,
                       GetScreenHeight() / 2 + 60, 20, WHITE);
        DrawTextCenter("<pressione ENTER para SALVAR sua classificação>",
                       GetScreenWidth() / 2, GetScreenHeight() - 90, 24, GRAY);
        DrawTextCenter("<pressione ESC para sair sem salvar",
                       GetScreenWidth() / 2, GetScreenHeight() - 70, 20, GRAY);

        EndDrawing();
    }
}

void menuTutorial(JOGO *jogo) {
    Vector2 origem = {0.0, 0.0};

    if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, -1);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2, 50, 60, WHITE);
    DrawTextCenter("Tutorial", GetScreenWidth() / 2, 120, 30, GRAY);
    DrawTextCenter("<pressione ESC para voltar>", GetScreenWidth() / 2,
                   GetScreenHeight() - 30, 15, GRAY);

    origem = (Vector2){215, 230};
    desenharSpriteTutorial(
        "Jogador",
        "O jogador é você! Use as setas para se mover e colete "
        "créditos para abrir a saída e escape do labirinto antes do "
        "tempo acabar.",
        jogo->texturas.jogador, origem, 1);

    origem.y += 150;
    desenharSpriteTutorial(
        "Professor",
        "O professor é importante para sua saída do labirinto! "
        "Responda suas perguntas corretamente e receba crédtios",
        jogo->texturas.professor, origem, 1);

    origem.y += 150;
    desenharSpriteTutorial(
        "Colega",
        "Ajude os colegas que você encontrar pelo caminho e receba "
        "recompensas! Você pode ganhar tempo extra, vidas ou bombas.",
        jogo->texturas.colega, origem, 0);

    origem = (Vector2){535, 230};
    desenharItemTutorial("Crédito",
                         "Colete créditos para abrir a saída do "
                         "labirinto e escape antes do tempo acabar!",
                         jogo->texturas.credito, origem);

    origem.y += 125;
    desenharItemTutorial(
        "Vida", "Caso você perca todas as suas vidas, você perde o jogo!",
        jogo->texturas.vida, origem);

    origem.y += 125;
    desenharItemTutorial("Relógio", "Colete relógios para ganhar tempo extra!",
                         jogo->texturas.relogio, origem);

    origem.y += 125;
    desenharItemTutorial(
        "Bomba",
        "Use bombas para destruir paredes e abrir caminhos, mas cuidado "
        "para "
        "não se explodido por ela! Use <ESPAÇO> para colocar uma.",
        jogo->texturas.bombaItem, origem);

    // Use + e - para aumentar e diminuir o volume da música
    DrawTextCenter(
        "Use + e - (do teclado número) para aumentar e diminuir o volume da "
        "música e M para mutar",
        GetScreenWidth() / 2, GetScreenHeight() - 52, 17, GRAY);
    EndDrawing();
}

void menuClassificacao(JOGO *jogo) {
    int i = 0, c = 0;
    Color corDestaque = WHITE;
    FILE *arquivo;

    if (IsKeyPressed(KEY_ESCAPE)) mudarCena(jogo, CENA_MENU_PRINCIPAL);
    alterarItemSelecionado(&jogo->itemSelecionado, 5);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextCenter("Os labirintos do INF", GetScreenWidth() / 2, 100, 60,
                   WHITE);
    DrawTextCenter("Classificação", GetScreenWidth() / 2, 170, 30, COLOR_GREEN);

    DrawTextCenter("<use as setas para alternar a dificuldade>",
                   GetScreenWidth() / 2, GetScreenHeight() - 50, 15, GRAY);

    DrawTextCenter("<pressione ESC para voltar ao menu principal>",
                   GetScreenWidth() / 2, GetScreenHeight() - 30, 15, GRAY);

    // Desenha a dificuldade selecionada
    switch (jogo->itemSelecionado) {
        case 0:
            DrawTextCenter("      Fácil      >", GetScreenWidth() / 2, 205, 30,
                           GRAY);
            break;
        case 1:
            DrawTextCenter("<     Médio      >", GetScreenWidth() / 2, 205, 30,
                           GRAY);
            break;
        case 2:
            DrawTextCenter("<     Difícil    >", GetScreenWidth() / 2, 205, 30,
                           GRAY);
            break;
        case 3:
            DrawTextCenter("<   Impossível   >", GetScreenWidth() / 2, 205, 30,
                           GRAY);
            break;
        case 4:
            DrawTextCenter("<      Geral       ", GetScreenWidth() / 2, 205, 30,
                           COLOR_GOLD);
            break;
    }

    // Desenha o ranking, enquanto não chegar ao fim ou não desenhar 15
    while (i < jogo->qtdeRanks && c < 15) {
        // Desenha apenas os jogadores da dificuldade selecionada ou geral
        if (jogo->ranking[i].dificuldade - 1 == jogo->itemSelecionado ||
            jogo->itemSelecionado == 4) {
            // Se for desenhar, incrementa o contador
            c++;

            corDestaque = strcmp(jogo->ranking[i].nome, jogo->jogador.nome) == 0
                              ? COLOR_GOLD
                              : WHITE;

            // Desenha a colocação, nome e pontuação
            DrawText(TextFormat("%2dº", c), 200, 230 + c * 30, 30, GRAY);
            DrawText(jogo->ranking[i].nome, 260, 230 + c * 30, 30, corDestaque);
            DrawText(TextFormat("%dpts", jogo->ranking[i].pontuacao), 650,
                     230 + c * 30, 30, corDestaque);
        }

        // Incrementa o contador
        i++;
    }

    EndDrawing();
}