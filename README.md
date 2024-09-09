# Os Labirintos do INF
**Autores:** Adriel de Souza e João Pedro Müller Alvarenga

**Orientadores:** Dennis Giovani Balreira e Santiago Valdes Ravelo

----
Inspirado no jogo **"Laberinto del Saber"**, jogo criado em Havana/Cuba no ano 1988 e relançado em 2013 como "Maze of Knowledge", Os Labirintos do INF **é um jogo educativo que mistura elementos de labirinto e quiz**. O jogador deve encontrar a saída do labirinto antes do tempo acabar e responder perguntas feitas pelos NPCs para obter créditos necessários para prosseguir.

![labirinto.png](/.github/labirinto.png)

A iniciar um novo labirinto, seja por iniciar um novo jogo ou avançar o nível, o labirinto é carregado e populado automaticamente com bombas, relógios, vidas, créditos, colegas e professores:

- **Bombas:** Colete bombas pelo labirinto, use ESPAÇO para colocar uma bomba para destruir paredes e abrir caminhos. Cuidado: As bombas destroem **tudo** ao seu redor (isso include o jogador)!
- **Relógio:** Colete relógios para ganhar 30 segundos extras no para sair do labirinto
- **Vidas:** Colete vidas para ter mais chances de sair o labirinto
- **Créditos:** Colete créditos para liberar a saída do labirinto, porém os créditos não serão suficientes por si só é necessário responder as perguntas dos professores.
- **Colegas:** Os colegas representam jogadores que não conseguiram sair do labirinto, ao acabar suas vidas uma versão do seu jogador ficará preso no labirinto até que outro jogador responda sua pergunta.
- **Professores:** Os professores andam pelo mapa e perseguem o jogador, ao encontrar um jogador o professor faz uma pergunta que deve ser respondida corretamente para receber um crédito, caso contrário você perderá uma vida.

Ao zerar suas vidas, o jogador terá a opção de **salvar sua pontuação** na lista de ganhadores e, caso possuir pontuação suficiente, terá seu nome exibido na lista.

O jogador pode, a qualquer momento durante o jogo, salvar o jogo ou carregar um jogo previamente salvo.

## Imagens do Jogo

![tela_inicial.png](/.github/tela_inicial.png)
![tutorial.png](/.github/tutorial.png)
![dificuldades.png](/.github/dificuldades.png)
![labirinto.png](/.github/labirinto.png)
![perguntas.png](/.github/perguntas.png)
![salvar.png](/.github/salvar.png)
![leaderboard.png](/.github/leaderboard.png)
![creditos.png](/.github/creditos.png)

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

💡 As estruturas também podem vistas diretamente no arquivo [**/parts/base.h**](/parts/base.h), este que inclui também as constantes usadas no programa
