#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

#define DELAY 30000
#define BORDA "*"

int main() {
    int xAlien, yAlien, xPlayer, yPlayer;
    int i, key, max_x, max_y, direction = 1;
    getmaxyx(stdscr, max_y, max_x);

    // Posicoes Iniciais
    xAlien = 2;
    yAlien = max_y + 4;
    xPlayer = 50; // Por enquanto ta fixo, tem que ser relativo a max_x e max_y
    yPlayer = 50;

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

    while(1){
        // Limpa o console
        clear();

        // Tamanho da tela recalculado a cada loop (responsivo)
        getmaxyx(stdscr, max_y, max_x);
        // Score
        mvprintw(1, 2, "SCORE: XXX");

        // Bordas Verticais
        for(i = 3; i < max_y; i++){
            mvprintw(i, 1, BORDA);
            mvprintw(i, max_x - 2, BORDA);
        }
        // Bordas Horizontais
        for(i = 1; i < max_x - 1; i++){
            mvprintw(3, i, BORDA);
            mvprintw(max_y - 1, i, BORDA);
        }

        // Aliens
        mvprintw(yAlien, xAlien, "@ @");
        mvprintw(yAlien + 1, xAlien, " @");

        // Player
        mvprintw(yPlayer, xPlayer, " @");
        mvprintw(yPlayer + 1, xPlayer, "@@@");

        key = getch();

        if((key == KEY_RIGHT) && (xPlayer + 6 <= max_x)){
            xPlayer++;
        }
        else if(key == KEY_LEFT && (xPlayer - 3 >= 0)){
            xPlayer--;
        }

        // manda para o console, "print real"
        refresh();

        // Tempo de 'recarga' das animacoes
        usleep(DELAY);

        // Colisoes com as Bordas
        if(xAlien + 5 >= max_x){
            direction = -1; // Esquerda
            yAlien += 1;
        }
        else if(xAlien - 2 <= 0){
            direction = 1; // Direita
            yAlien += 1;
        }
        xAlien += direction;
    }

    // para o ncurses
    endwin();

    return 0;
}
