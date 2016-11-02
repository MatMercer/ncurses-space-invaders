#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

/* Constantes */

// Delay de cada frame
#define DELAY 10000

// Como tudo deve ser printado
#define BORDA "*"
//#define NAVES "@" // not used

// Tamanho minimo da window
#define MIN_WINX 80
#define MIN_WINY 30

// Aliens
#define ALIENS_ROWS 5
#define ALIENS_COLUMNS 5

// Macro para a direcao dos movimentos
#define RIGHT 1
#define LEFT -1
#define UP -2
#define DOWN 2
#define NONE 0

// Define a quantidade de lasers no jogo
#define MAX_LASERS 2

/* Definicoes e construtores */

// Struct de posicao
typedef struct position {
    int x;
    int y;
} vec2;

// Struct de area
typedef struct areas {
    int x1;
    int x2;
    int y1;
    int y2;
} area;

// Struct de componente, posicao, vivo ou morto e direcao
typedef struct components {
    vec2 pos;
    bool isAlive;
    int direction;
} component;

// Inicia o jogo e configura o ncurses
void initGame();

// Define a posicao inicial do player
void initPlayer();

// Inicializa a matriz dos aliens
void initAliens();

// Inicia os lasers nas suas posicoes
void initLasers();

// Comeca o loop do jogo
// Chama a funcao getPressedKey()
// Chama a funcao getWinSize()
// Chama as funcoes de movimento
void play();

// Pega a tecla pressionada no momento e manda para PRESSED_KEY
void getPressedKey();

// Pega o tamanho da tela atual e manda para WIN_SIZE
void getWinSize();

// Para o jogo, mostrando uma mensagem se ganhou
// ou nao. Para o ncurses apos isso
void gameOver(bool winner);

// Movimenta o player, acoes baseadas em PRESSED_KEY
// A posicao eh baseada em PLAYER_POS
// Detecta colisoes, muda a posicao do player se necessario
void playerMovement();

// Movimenta os aliens
// Detecta colisoes com a parede e modifica o ALIENS_DIRECTION
void aliensMovement();

// Detecta se o jogador quer atirar e chama lasersMovement()
void playerShoot();

// Desenha os tiros dos aliens
void aliensShoot();

// Movimenta os lasers
void lasersMovement();

// Renderiza
// Chama as funcoes draw*() a cada loop
void render();

// Desenha as bordas do jogo
// Manda o tamanho da borda para BORDER_SIZE
void drawBorder();

// Desenha uma matriz de aliens
void drawAliens();

// Desenha um player na posicao dada
void drawPlayer();

// Desenha os lasers em suas posicoes
void drawLasers();

// Movimenta um componente baseado em sua direcao se ele estiver vivo
void moveComponent(component *comp);

/* Variaveis globais */

// vec2 zerado, evita lixo de memoria
// const vec2 INIT_VEC2; // not used

// Tamanho da tela
vec2 WIN_SIZE;

// Area da borda
area BORDER_AREA;

// Posicao do player
vec2 PLAYER_POS;

// Posicao dos aliens
component **ALIENS;

// Posicao dos lasers
vec2 LASER_POS[MAX_LASERS];

// Diz se existe um disparo do player em andamento
bool IS_PLAYER_SHOOTING;

// Score
unsigned int SCORE;

// Direcao dos aliens, muda de acordo com as colisoes
int ALIENS_DIRECTION;

// True para normal, false para gameover
bool GAME_STATUS;

// Qual tecla esta sendo pressionada no momento
int PRESSED_KEY;

// Um timer global, eh incrementado a cada loop da funcao play()
unsigned long int GLOBALTIME;

//TODO: Teste de detecão de colisao, remover isso no futuro
chtype DETECTED_CHAR;

// Usado para debug
#ifdef DEBUG

void drawDebug();

#endif
