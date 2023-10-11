#include "arquivos.h"
/*
Todas as funções que lidam com arquivos estão aqui.
*/

// Carregar o labirinto
void carregarLabirinto(JOGO *jogo) {
    FILE *arquivo;
    int i, deslocamento;

    // Abre o arquivo
    arquivo = fopen(FILE_LABIRINTOS, "rb");

    // Se o arquivo for inválido
    if (arquivo == NULL) {
        strcpy(jogo->mensagem, "Erro ao abrir o arquivo do labirinto");
        mudarCena(jogo, CENA_SAIR);
    } else {
        // Lê a quantidade de labirintos
        fread(&jogo->qtdeLabirintos, sizeof(int), 1, arquivo);

        // Atualiza o cursor do arquivo
        deslocamento = sizeof(jogo->labirintoAtual) *
                       ((jogo->nivel - 1) % jogo->qtdeLabirintos);
        fseek(arquivo, deslocamento, SEEK_CUR);

        // Lê o labirinto atual
        fread(&jogo->labirintoAtual, sizeof(jogo->labirintoAtual), 1, arquivo);
    }
    // Fechar o arquivo
    fclose(arquivo);
}

// Carregar as perguntas
void carregarPerguntas(PERGUNTA lista[], int *qtde_perguntas) {
    FILE *arquivo;
    int q = 0, i;
    char pergunta_[MAX_TEXTO];

    // Abre o arquivo
    arquivo = fopen(FILE_PERGUNTAS, "r");

    if (arquivo == NULL) {
        *qtde_perguntas = -1;
    } else {
        // Enquanto conseguir ler uma linha
        while ((fgets(pergunta_, MAX_TEXTO, arquivo)) != NULL && q < MAX_PERGUNTAS) {
            
            strcpy(lista[q].pergunta, pergunta_);
            lista[q].ja_respondida = 0;

            // Lê e armazena a qtde de opcoes e a respotas correta
            fscanf(arquivo, "%d\n%d\n", &lista[q].n_alternativas,
                   &lista[q].resposta_correta);

            // Itera, lê e armazena as alternativas
            for (i = 0; i < lista[q].n_alternativas; i++) {
                fgets(lista[q].alternativas[i], MAX_OPTLEN, arquivo);
            }

            q++;
        }

        *qtde_perguntas = q;
    }

    // Fechar o arquivo
    fclose(arquivo);
}


int carregarClassificacao(RANKING lista[]) {
    FILE *arquivo;
    int qtdeJogadores = 0;

    // Abre o arquivo
    arquivo = fopen(FILE_RANKING, "rb");

    if (arquivo != NULL) {
        // Lê os jogadores salvos
        while (!feof(arquivo) && qtdeJogadores < MAX_RANKING) {
            if (fread(&lista[qtdeJogadores], sizeof(RANKING), 1, arquivo) == 1)
                qtdeJogadores++;
        }
    }

    fclose(arquivo);

    return qtdeJogadores;
}


int salvarJogadorComoColega(JOGADOR j, int nivel) {
    COLEGA colegas[MAX_COLEGAS_SALVOS];
    int qtdeCarregados;

    // Carrega os colegas salvos
    qtdeCarregados = carregarColegas(colegas, -1, NULL);

    // Se atingiu o limite de colegas salvos, sobreescreve o ultimo
    if(qtdeCarregados == MAX_COLEGAS_SALVOS)
        qtdeCarregados--;
    
    // Adiciona o jogaodr e incrementa a quantidade de colegas carregados
    colegas[qtdeCarregados++] = (COLEGA){j.pos, nivel, 0};

    // Salva e retorna a quantidade de colegas salvos
    return salvarColegas(colegas, qtdeCarregados);
}

int salvarColegas(COLEGA lista[], int qtde) {
    int qtdeSalva = qtde;
    FILE *arquivo;

    // Reescreve o arquivo
    arquivo = fopen(FILE_COLEGAS, "wb");
    if (arquivo != NULL) {
        if (fwrite(lista, sizeof(COLEGA), qtde, arquivo) != qtde) {
            TraceLog(LOG_ERROR, "Erro ao salvar os colegas");
            qtdeSalva = -1;
        }
    } else {
        TraceLog(LOG_ERROR, "Erro ao abrir o arquivo para salvar os colegas");
        qtdeSalva = -1;
    }

    fclose(arquivo);

    return qtdeSalva;
}

// Carrega os colegas no labirinto
int carregarColegas(COLEGA listaColegas[], int nivel,
                    char lab[TAM_LAB][TAM_LAB]) {
    int qtdColegas = 0, i;
    FILE *arquivo;
    COLEGA colegaCarregado;
    Vector2 pos;
    // Carrega os colegas do arquivo binário
    arquivo = fopen(FILE_COLEGAS, "rb");

    if (arquivo != NULL) {
        while (!feof(arquivo)) {
            if (fread(&colegaCarregado, sizeof(COLEGA), 1, arquivo) == 1) {
                // Se o colega for do mesmo nivel, adiciona na lista ou se o
                // nivel for -1, adiciona todos os colegas
                if (colegaCarregado.nivel == nivel || nivel == -1) {
                    listaColegas[qtdColegas] = colegaCarregado;
                    qtdColegas++;
                }
            }
        }
    } else {
        TraceLog(LOG_ERROR, "Erro ao abrir o arquivo para salvar os colegas");
    }

    fclose(arquivo);

    // Se for informado um labirinto e quantidade de colegas carregado for menor
    // que 3 adicona colegas aleatórios no labirinto
    while (qtdColegas < 3 && lab != NULL) {
        // Sorteia uma posição aleatória
        pos = (Vector2){GetRandomValue(0, TAM_LAB - 1),
                        GetRandomValue(0, TAM_LAB - 1)};
        // Se a posição sorteada for vazia, adiciona o colega
        if (lab[(int)pos.y][(int)pos.x] == ' ') {
            listaColegas[qtdColegas] = (COLEGA){pos, -1, 0};

            qtdColegas++;
        }
    }

    return qtdColegas;
}


int salvarJogo(JOGO *jogo) {
    FILE *arquivo;
    SAVE save;
    int sobreescrever = 0, salvou = 0, qtde = 0;

    arquivo = fopen(FILE_SAVES, "rb+");
    if (arquivo != NULL) {
        // Verifica se o já existe um save com o nome do jogador
        while (!feof(arquivo) && !sobreescrever) {
            if (fread(&save, sizeof(SAVE), 1, arquivo) == 1) {
                if (strcmp(save.jogador.nome, jogo->jogador.nome) == 0) {
                    // Se existir, sobreescreve o save
                    fseek(arquivo, -sizeof(SAVE), SEEK_CUR);
                    sobreescrever = 1;
                }
                qtde++;
            }
        }

        // Se a quantidade de saves for maior que o máximo, sobrecreve o
        // último save
        if (qtde >= MAX_SAVES) fseek(arquivo, -sizeof(SAVE), SEEK_END);

        // Gera um save e salva no arquivo
        save = gerarSave(jogo);
        salvou = fwrite(&save, sizeof(SAVE), 1, arquivo);
    }

    fclose(arquivo);
    return salvou;
}


int carregarSaves(SAVE lista[]) {
    FILE *arquivo;
    int qtde = 0;

    arquivo = fopen(FILE_SAVES, "rb");
    // Percorre o arquivo e salva os saves na lista
    if (arquivo != NULL) {
        while (!feof(arquivo)) {
            if (fread(&lista[qtde], sizeof(SAVE), 1, arquivo) == 1) qtde++;
        }
    }

    fclose(arquivo);
    return qtde;
}


SAVE gerarSave(JOGO *jogo) {
    SAVE save;
    int i, j;

    save.qtdeProfessores = jogo->qtdeProfessores;
    save.qtdeColegas = jogo->qtdeColegas;
    save.qtdeBombasAtivas = jogo->qtdeBombasAtivas;
    save.nivel = jogo->nivel;
    save.dificuldade = jogo->dificuldade;
    save.creditosNecessarios = jogo->creditosNecessarios;
    save.tempoMaximo = jogo->tempoMaximo;
    save.tempoRestante = jogo->tempoRestante;
    save.jogador = jogo->jogador;

    for (i = 0; i < jogo->qtdeBombasAtivas; i++)
        save.bombasAtivas[i] = jogo->bombasAtivas[i];

    for (i = 0; i < jogo->qtdeProfessores; i++)
        save.professores[i] = jogo->professores[i];

    for (i = 0; i < jogo->qtdeColegas; i++) save.colegas[i] = jogo->colegas[i];

    for (i = 0; i < TAM_LAB; i++)
        for (j = 0; j < TAM_LAB; j++)
            save.labirintoAtual[i][j] = jogo->labirintoAtual[i][j];

    return save;
}


int salvarClassificacao(JOGADOR jogador, int dificuldade, int *cena,
                        char mensagemErro[]) {
    FILE *arquivo;
    int i, pontos, qtdeJogadores = 0, colocacao = -1;
    char nome[MAX_NOME_JOGADOR];
    RANKING lista[MAX_RANKING + 1];

    // Abre o arquivo
    arquivo = fopen("./data/ranking.bin", "rb+");

    if (arquivo != NULL) {
        // Lê os jogadores salvos
        while (!feof(arquivo) && qtdeJogadores < MAX_RANKING) {
            if (fread(&lista[qtdeJogadores], sizeof(RANKING), 1, arquivo) == 1)
                qtdeJogadores++;
        }

        // Adiciona o jogado atual na lista
        lista[qtdeJogadores].pontuacao = jogador.pontuacao;
        strcpy(lista[qtdeJogadores].nome, jogador.nome);
        lista[qtdeJogadores].dificuldade = dificuldade;

        // Incrementa a quantidade de jogadores
        qtdeJogadores++;

        // Ordena a lista
        ordernarRanking(lista, qtdeJogadores);

        // Obtem a colocacao do jogador
        for (i = 0; i < qtdeJogadores; i++)
            if (strcmp(lista[i].nome, jogador.nome) == 0) colocacao = i + 1;

        rewind(arquivo);  // Volta ao inicio do arquivo

        // Reescreve o arquivo
        if (fwrite(lista, sizeof(RANKING), qtdeJogadores, arquivo) !=
            qtdeJogadores) {
            {
                TraceLog(LOG_ERROR, "Erro ao salvar o ranking");
                *cena = -1;
                strcpy(mensagemErro, "Erro ao abrir o arquivo!");
            }
        }
    } else {
        TraceLog(LOG_ERROR, "Erro ao abrir o arquivo para carregar");
        *cena = -1;
        strcpy(mensagemErro, "Erro ao abrir o arquivo!");
    }

    fclose(arquivo);

    return colocacao;
}


int removerColegaDoArquivo(COLEGA c) {
    FILE *arquivo;
    COLEGA colegas[MAX_COLEGAS_SALVOS];
    int i, j, sucesso = 0, qtdeCarregados = 0;

    if (c.nivel != -1) {
        // Carrega os colegas salvos
        qtdeCarregados = carregarColegas(colegas, -1, NULL);

        // Se tiver colegas salvos
        if (qtdeCarregados > 0) {
            for (i = 0; i < qtdeCarregados; i++) {
                // Se o colega nova lista lista for igual a um colega iá
                // salvo, e esse colega iá tiver sido respondido pelo
                // iogador, remove ele da lista
                if (vectorEqual(c.pos, colegas[i].pos) &&
                    c.nivel == colegas[i].nivel) {
                    // Remove o colega da lista de colegas carregados,
                    // deslocando os seguintes para a esquerda
                    for (int j = i; j < qtdeCarregados - 1; j++)
                        colegas[j] = colegas[j + 1];
                    qtdeCarregados--;
                }
            }
        }

        // Salva e retorna a quantidade de colegas salvos
        if (salvarColegas(colegas, qtdeCarregados) != -1) sucesso = 1;
    }

    return sucesso;
}
