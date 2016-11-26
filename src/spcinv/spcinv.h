/**
 * @file spcinv.h
 * @author Gabriel Ranéa, Mateus Mercer
 * @date 19 Nov 2016
 * @brief Biblioteca para o jogo Space Invaders.
 *
 * Essa biblioteca define o que o jogo executa, entre elas
 * estão funções de renderização, um loop principal e de interface
 * com o usuário
 */

#ifndef SPACE_INVADERS_GAME_SPCINV_H
#define SPACE_INVADERS_GAME_SPCINV_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>

/**
 * @brief Delay de cada frame
 */
#define DELAY 10000

/**
 * @brief Pontuacao por alien atingido
 */
#define KILL_SCORE 10

/**
 * @brief Tamanho minimo da window
 */
#define MIN_WINX 90
#define MIN_WINY 45

/**
 * @brief Macro para a direcao dos movimentos
 */
#define RIGHT 1
#define LEFT -1
#define UP -2
#define DOWN 2

/**
 * @brief Define a quantidade de lasers no jogo
 */
#define MAX_LASERS 4

/**
 * @brief Aonde os sfx estao
 */
#define SFX_PATH "./sfx/"

/**
 * @brief Como eles sao tocados
 */
#define SFX_COMMAND "play -q"

/**
 * @brief Qual extensão eles usam
 */
#define SFX_EXTENSION ".wav"

/**
 * @brief Struct de posicao
 */
typedef struct position {
    int x;
    int y;
} vec2;

/**
 * @brief Struct de area
 */
typedef struct areas {
    int x1;
    int x2;
    int y1;
    int y2;
} area;

/**
 * @brief Struct de componente, posicao, vivo ou morto e direcao
 */
typedef struct components {
    vec2 pos;
    bool isAlive;
    int direction;
} component;

/**
 * @brief Inicia o jogo e configura o ncurses
 */
static void initGame();

/**
 * @brief Define a posicao inicial do player
 */
static void initPlayer();

/**
 * @brief Inicializa a matriz dos aliens
 */
static void initAliens();

/**
 * @brief Inicia os lasers nas suas posicoes
 */
static void initLasers();

/**
 * @brief Comeca o loop do jogo que chama as funcoes de movimento, acoes e colisoes
 */
extern void startSpaceInvaders();

/**
 * @brief Pega a tecla pressionada no momento e manda para PRESSED_KEY
 */
static void getPressedKey();

/**
 * @brief Pega o tamanho da tela atual e manda para WIN_SIZE
 */
static void getWinSize();

/**
 * @brief Para o jogo, mostrando uma mensagem se ganhou ou nao, para o ncurses apos isso
 */
static void gameOver(bool winner);

/**
 * @brief Movimenta o player por meio de PLAYER_POS, acoes baseadas em PRESSED_KEY
 */
static void playerMovement();

/**
 * @brief Movimenta os aliens, detecta colisoes com a parede e modifica o ALIENS_DIRECTION
 */
static void aliensMovement();

/**
 * @brief Detecta colisoes do player com os aliens ou lasers dos aliens
 */
static void playerLife();

/**
 * @brief Verifica aliens vs lasers
 */
static void aliensLife();

/**
 * @brief Detecta se o jogador quer atirar e chama playerLaser()
 */
static void playerShoot();

/**
 * @brief Desenha os tiros dos aliens
 */
static void aliensShoot();

/**
 * @brief Movimenta o laser do player
 */
static void playerLaser();

/**
 * @brief Renderiza e chama as funcoes draw*() a cada loop
 */
static void render();

/**
 * @brief Desenha as bordas do jogo e manda o tamanho da borda para BORDER_SIZE
 */
static void drawBorder();

/**
 * @brief Desenha uma matriz de aliens
 */
static void drawAliens();

/**
 * @brief Desenha um player na posicao dada
 */
static void drawPlayer();

/**
 * @brief Desenha os lasers em suas posicoes
 */
static void drawLasers();

/**
 * @brief Movimenta um componente baseado em sua direcao se ele estiver vivo
 */
static void moveComponent(component *comp);

/**
 * @brief Usado para tocar um sfx
 */
static void playSound(char *name);

/**
 * @brief Usado para descobrir aonde o executavel esta
 *
 * Baseado em http://advancedlinuxprogramming.com/listings/chapter-7/get-exe-path.c
 */
static void getWorkingDirectory (char* buffer, size_t size);

/**
 * @brief Tamanho da tela
 */
vec2 WIN_SIZE;

/**
 * @brief Area da borda
 */
area BORDER_AREA;

/**
 * @brief True para normal, false para gameover
 */
bool GAME_STATUS;

/**
 * @brief True para sons ativados, false para sons desativados
 */
bool SOUNDS_ENABLED;

/**
 * @brief Um timer global, eh incrementado a cada loop da funcao startSpaceInvaders()
 */
unsigned long int GLOBALTIME;

/**
 * @brief Como a borda deve se parecer
 */
char BORDER[1];

/**
 * @brief Posicao do player
 */
vec2 PLAYER_POS;

/**
 * @brief Diz se existe um disparo do player em andamento
 */
bool IS_PLAYER_SHOOTING;

/**
 * @brief Player possui vidas, que valem em colisoes com lasers
 */
unsigned int PLAYER_LIVES;

/**
 * @brief Qual tecla esta sendo pressionada no momento
 */
int PRESSED_KEY;

/**
 * @brief Score
 */
unsigned int SCORE;

/**
 * @brief Linhas e Colunas
 */
unsigned int ALIENS_ROWS, ALIENS_COLUMNS;

/**
 * @brief Posicao dos aliens
 */
component **ALIENS;

/**
 * @brief Direcao dos aliens, muda de acordo com as colisoes
 */
int ALIENS_DIRECTION;

/**
 * @brief Armazena o indice da ultima fileira de aliens e ultimo alien
 */
unsigned int LAST_ALIVE_ROW, LAST_ALIVE_ALIEN;

/**
 * @brief Posicao dos lasers
 */
vec2 LASER_POS[MAX_LASERS];

/**
 * @brief Usado para estocar o diretorio atual do executavel
 *
 * 4096 é baseado no tamanho máximo de bytes que um filepath pode ter no sistema de arquivos EXT
 */
char WORKING_DIR[4096];

/**
 * @brief Usado para debug
 */
#ifdef DEBUG

static void drawDebug();

#endif // DEBUG

#endif // SPACE_INVADERS_GAME_SPCINV_H
