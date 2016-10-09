#include <stdbool.h>

/* Constantes */

// Delay de cada frame
#define DELAY 30000

// Como tudo deve ser printado
#define BORDA "*"
#define NAVES "@"

// Definicao de direita e esquerda para tornar o codigo mais legivel
#define RIGHT TRUE
#define LEFT FALSE

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

// Inicia o jogo, configurando as posicoes inicias e o ncurses
void initGame();

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

// Movementa o player, acoes baseadas em PRESSED_KEY
// A posicao eh baseada em PLAYER_POS
// Detecta colisoes, muda a posicao do player se necessario
// Detecta se o jogador quer atirar
void playerMovement();

// Movementa os aliens
// Detecta colisoes com a parede e modifica o ALIEN_DIRECTION
void aliensMovement();

// Renderiza
// Chama as funcoes draw*() a cada loop
void render();

// Desenha as bordas do jogo
// Manda o tamanho da borda para BORDER_SIZE
void drawBorder();

// Desenha uma matriz de aliens
void drawAliens();

// Desenha um alien na posicao dada
void drawAlien();

// Desenha um player na posicao dada
void drawPlayer();

/* Variaveis globais */

// vec2 zerado, evita lixo de memoria
const vec2 INIT_VEC2;

// Tamanho da tela
vec2 WIN_SIZE;

// Area da borda
area BORDER_AREA;

// Posicao do player
vec2 PLAYER_POS;

// Posicao dos aliens
vec2 **ALIENS_POS;

// Score
unsigned int SCORE;

// Direcao dos aliens, muda de acordo com as colisoes
bool ALIEN_DIRECTION;

// True para normal, false para gameover
bool GAME_STATUS;

// Qual tecla esta sendo pressionada no momento
int PRESSED_KEY;

// Um timer global, eh incrementado a cada loop da funcao play()
unsigned long int GLOBALTIME;
