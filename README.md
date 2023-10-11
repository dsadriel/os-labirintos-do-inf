# Trabalho Prático Final - Semestre 2023/1


Prof. Santiago V. Ravelo
INF01202 - Algoritmos e Programação

# Os Labirintos do INF


### Uso básico do programa

- Use as setas direcionais, ENTER e ESC para alternar entre os menus
- Use W, A, S e D ou as setas direcionais para mover o jogador
- Use ESPAÇO para colocar bombas no mapa

O jogo se trata de uma combinação dos estilos **labirinto** e **quiz**, para avançar os níveis do jogo é necessário que você encontre a saída do labirinto antes do fim do tempo e acerte as perguntas feitas pelos NPCs (professores e colegas) para obter créditos necessários para prosseguir.

A iniciar um novo labirinto, seja por iniciar um novo jogo ou avançar o nível, o labirinto é carregado e populado automaticamente com bombas, relógios, vidas, créditos, colegas e professores:

- **Bombas:** Colete bombas pelo labirinto, use ESPAÇO para colocar uma bomba para destruir paredes e abrir caminhos. Cuidado: As bombas destroem **tudo** ao seu redor (isso include o jogador)!
- ******************Relógio:****************** Colete relógios para ganhar 30 segundos extras no para sair do labirinto
- **************Vidas:************** Colete vidas para ter mais chances de sair o labirinto
- ************Créditos:************ Colete créditos para liberar a saída do labirinto, porém os créditos não serão suficientes por si só é necessário responder as perguntas dos professores.
- ******************Colegas:****************** Os colegas representam jogadores que não conseguiram sair do labirinto, ao acabar suas vidas uma versão do seu jogador ficará preso no labirinto até que outro jogador responda sua pergunta.
- ****Professores:**** Os professores andam pelo mapa e perseguem o jogador, ao encontrar um jogador o professor faz uma pergunta que deve ser respondida corretamente para receber um crédito, caso contrário você perderá uma vida.

Ao zerar suas vidas, o jogador terá a opção de **salvar sua pontuação** na lista de ganhadores e, caso possuir pontuação suficiente, terá seu nome exibido na lista.

O jogador pode, a qualquer momento durante o jogo, salvar o jogo ou carregar um jogo previamente salvo.

## Configuração e estrutura do código

Para o melhor entendimento, dentro do possível, o código foi divido em partes e incluídas no jogo principal através de bibliotecas para cada “conjunto” de código.

| Arquivo | Principais funcionalidades |
| --- | --- |
| /os-labirintos-do-inf(.c/.h) | Contém as mecânicas principais do jogo |
| /parts/arquivos(.c/.h) | Contém todas as funções que lidam com arquivos |
| /parts/base.h | Contém todas as definições básicas do programa como constantes e estruturas |
| /parts/desenho(.c/.h) | Contém as funções relacionadas a desenho na tela |
| /parts/menus(.c/.h) | Contém as funções que desenham os menus |
| /parts/utilidades(.c/.h) | Contém diversas funções utilitárias mais básicas |

## Estruturas do programa

<aside>
💡 As estruturas também podem vistas diretamente no arquivo **/parts/base.h**, este que inclui também as constantes usadas no programa

</aside>

```c
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
```