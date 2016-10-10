#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

#include "spcinv.h"

void initGame() {
    // Inicializa o ncurses
    initscr();

    // Configuracoes ncurses
    // Nao printe caracteres digitados
    noecho();
    // Desabilite o cursor do console
    curs_set(FALSE);
    // Leia caracteres especias como F1...F12 da maneira correta
    keypad(stdscr, TRUE);
    // Desabilite o delay de espera para getch
    nodelay(stdscr,TRUE);

    // Comeca a contagem do GLOBALTIME
    GLOBALTIME = 0;

    // Jogo nao esta em gameover
    GAME_STATUS = TRUE;

    // Pega o tamanho da tela
    getWinSize();

    // Desenha as bordas
    drawBorder();

    // O player comeca no canto esquerdo
    PLAYER_POS.x = BORDER_AREA.x1 + 2;
}

void play() {
    // Enquanto nao for gameover...
    while(GAME_STATUS) {
        // Pega qual tecla esta sendo pressionada
        getPressedKey();

        // Movimenta o jogo
        playerMovement();
        aliensMovement();

        // Renderiza
        render();

        // Incrementa o GLOBALTIME
        GLOBALTIME++;

        // Tempo de 'recarga'
        usleep(DELAY);
    }
}

void render() {
    // Limpa o console
    clear();

    // Tamanho da tela recalculado a cada loop (responsivo)
    getWinSize();

    // Renderiza os objetos do jogo
    drawBorder();
    drawPlayer();
    drawAliens();

    // Manda para o console
    refresh();
}

void getPressedKey() {
    PRESSED_KEY = getch();
}

void getWinSize() {
    // Tamanho da tela
    getmaxyx(stdscr, WIN_SIZE.y, WIN_SIZE.x);
}

void drawBorder() {
    int i;

    // Tamanho da area da borda
    BORDER_AREA.x1 = 1;
    BORDER_AREA.x2 = WIN_SIZE.x - 1;
    BORDER_AREA.y1 = 3;
    BORDER_AREA.y2 = WIN_SIZE.y - 1;

    // Bordas Verticais
    for(i = BORDER_AREA.y1; i < BORDER_AREA.y2; i++){
        mvprintw(i, BORDER_AREA.x1, BORDA);
        mvprintw(i, BORDER_AREA.x2, BORDA);
    }

    // Bordas Horizontais
    for(i = BORDER_AREA.x1; i <= BORDER_AREA.x2; i++){
        mvprintw(BORDER_AREA.y1, i, BORDA);
        mvprintw(BORDER_AREA.y2, i, BORDA);
    }
}

void drawPlayer() {
    mvprintw(PLAYER_POS.y, PLAYER_POS.x, " @");
    mvprintw(PLAYER_POS.y + 1, PLAYER_POS.x, "@@@");
}

void drawAliens() {
}

void playerMovement() {
    // Manda o player sempre para a parte de baixo da borda
    PLAYER_POS.y = BORDER_AREA.y2 - 2;

    // Movimenta o player para a direita ou esquerda
    if((PRESSED_KEY == KEY_RIGHT) && (PLAYER_POS.x + 4 <= BORDER_AREA.x2)) {
        PLAYER_POS.x += 1;
    }
    else if((PRESSED_KEY == KEY_LEFT) && (PLAYER_POS.x - 2 >= BORDER_AREA.x1)) {
        PLAYER_POS.x -= 1;
    }
}

void aliensMovement() {
}
