#include <stdio.h>
#include <ncurses.h>

int main() {
    // inicializa o ncurses
    initscr();

    // printa para o buffer do ncurses, note que nao foi "renderizado" ainda;
    printw("Hello World!");

    // manda para o console, "print real"
    refresh();

    // espera pelo input to usuario
    getch();

    // para o ncurses
    endwin();

    return 0;
}
