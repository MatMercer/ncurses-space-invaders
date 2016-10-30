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
    nodelay(stdscr, TRUE);

    // Comeca a contagem do GLOBALTIME
    GLOBALTIME = 0;

    // Jogo nao esta em gameover
    GAME_STATUS = TRUE;

    SCORE = 0;

    // Pega o tamanho da tela
    getWinSize();

    // Desenha as bordas
    drawBorder();

    // Inicia o player
    initPlayer();

    // Inicia os aliens
    initAliens();

    // Inicia os lasers
    initLasers();
}

void initPlayer() {
    // O player comeca no meio e embaixo
    PLAYER_POS.x = WIN_SIZE.x / 2;
    PLAYER_POS.y = BORDER_AREA.y2 - 2;
}

void initAliens() {
    // Variaveis de loop
    int i, j;

    // Aloca os aliens
    ALIENS_POS = calloc(ALIENS_ROWS, sizeof(vec2 *));
    for (i = 0; i < ALIENS_ROWS; i++) {
        ALIENS_POS[i] = calloc(ALIENS_COLUMNS, sizeof(vec2));
    }

    // Coloca os aliens na posicao inicial
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            ALIENS_POS[i][j].x = 1 + BORDER_AREA.x1 + (3 * i);
            ALIENS_POS[i][j].y = 1 + BORDER_AREA.y1 + (2 * j);
        }
    }

    // Manda eles andarem para a direita
    ALIEN_DIRECTION = RIGHT;
}

void initLasers() {
    // Laser[0] comeca em cima do player
    LASER_POS[0].y = PLAYER_POS.y - 1;

    // Laser[1] comeca embaixo dos aliens
    LASER_POS[1].y = ALIENS_POS[4][4].y + 2;
    LASER_POS[1].x = ALIENS_POS[4][4].x;
}

void play() {
    // Enquanto nao for gameover...
    while (GAME_STATUS) {
        // Pega qual tecla esta sendo pressionada
        getPressedKey();

        // Movimenta o jogo
        playerMovement();
        playerShoot();

        if (GLOBALTIME % 10 == 0) {
            aliensMovement();
            aliensShoot();
        }

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

    // Contagem de Pontos
    mvprintw(BORDER_AREA.y1 - 2, BORDER_AREA.x1, "SCORE: %u", SCORE);

#ifdef DEBUG
    // Debug
    drawDebug();
#endif

    // Tamanho da tela recalculado a cada loop (responsivo)
    // Por enquanto desativado, quebra o jogo
    // getWinSize();

    // Renderiza os objetos do jogo
    drawBorder();
    drawPlayer();
    drawAliens();
    drawLasers();

    // Manda para o console
    refresh();
}

void getPressedKey() {
    PRESSED_KEY = getch();
}

void getWinSize() {
    // Tamanho da tela
    getmaxyx(stdscr, WIN_SIZE.y, WIN_SIZE.x);

    // Verifica se a WIN_SIZE nao esta menor que o tamanho minimo
    if (WIN_SIZE.x < MIN_WINX) {
        WIN_SIZE.x = MIN_WINX;
    }
    if (WIN_SIZE.y < MIN_WINY) {
        WIN_SIZE.y = MIN_WINY;
    }
}

void drawBorder() {
    int i;

    // Tamanho da area da borda
    BORDER_AREA.x1 = WIN_SIZE.x / 2 - WIN_SIZE.x / 3;
    BORDER_AREA.x2 = WIN_SIZE.x / 2 + WIN_SIZE.x / 3;
    BORDER_AREA.y1 = WIN_SIZE.y / 2 - WIN_SIZE.y / 3;
    BORDER_AREA.y2 = WIN_SIZE.y / 2 + WIN_SIZE.y / 3;

    // Bordas Verticais
    for (i = BORDER_AREA.y1; i < BORDER_AREA.y2; i++) {
        mvprintw(i, BORDER_AREA.x1, BORDA);
        mvprintw(i, BORDER_AREA.x2, BORDA);
    }

    // Bordas Horizontais
    for (i = BORDER_AREA.x1; i <= BORDER_AREA.x2; i++) {
        mvprintw(BORDER_AREA.y1, i, BORDA);
        mvprintw(BORDER_AREA.y2, i, BORDA);
    }
}

// TODO: Arrumar a pos do player eh maior que o x da tela
// o player buga pro outro lado quando isso acontece
void drawPlayer() {
    mvprintw(PLAYER_POS.y, PLAYER_POS.x, " @");
    mvprintw(PLAYER_POS.y + 1, PLAYER_POS.x, "@@@");
}

void drawAliens() {
    int i, j;

    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            mvprintw(ALIENS_POS[i][j].y, ALIENS_POS[i][j].x, "@ @");
            mvprintw(ALIENS_POS[i][j].y + 1, ALIENS_POS[i][j].x, " @");
        }
    }
}

void drawLasers() {
    // Se nao estiver em cima do player
    if (LASER_POS[0].y != PLAYER_POS.y - 1) {
        // Limpa posicao anterior do laser
        mvprintw(LASER_POS[0].y + 1, LASER_POS[0].x, " ");
        // Desenha o laser disparado pelo player
        mvprintw(LASER_POS[0].y, LASER_POS[0].x, "|");
    }
    // Desenha o laser dos aliens
    mvprintw(LASER_POS[1].y, LASER_POS[1].x, "|");
}

#ifdef DEBUG

char *dirToString(int dir) {
    switch (dir) {
        case 1:
            return "RIGHT";
        case -1:
            return "LEFT";
        case 2:
            return "DOWN";
        case -2:
            return "UP";
        default:
            return "UNDEFINED";
    }
}

void drawDebug() {
    mvprintw(BORDER_AREA.y1 - 8, BORDER_AREA.x1, "WINDOW SIZE: %d:%d \tBORDER AREA: %d:%d:%d:%d \tGAME STATUS: %d",
             WIN_SIZE.x, WIN_SIZE.y, BORDER_AREA.x1, BORDER_AREA.x2, BORDER_AREA.y1, BORDER_AREA.y2, GAME_STATUS);
    mvprintw(BORDER_AREA.y1 - 6, BORDER_AREA.x1, "GLOBALTIME: %ld \t\tPLAYER POS: %d:%d\t\tPRESSED KEY: %d", GLOBALTIME,
             PLAYER_POS.x, PLAYER_POS.y, PRESSED_KEY);
    mvprintw(BORDER_AREA.y1 - 4, BORDER_AREA.x1, "ALIENS DIRECTION: %s\tLASER POS: %d:%d ",
             dirToString(ALIEN_DIRECTION), LASER_POS[0].x, LASER_POS[0].y);
}

#endif

void playerMovement() {
    // Manda o player sempre para a parte de baixo da borda
    PLAYER_POS.y = BORDER_AREA.y2 - 2;

    // Verifica se o player nao esta fora da borda
    if (PLAYER_POS.x + 4 > BORDER_AREA.x2) {
        PLAYER_POS.x = BORDER_AREA.x2 - 3;
    } else if (PLAYER_POS.x < BORDER_AREA.x1) {
        PLAYER_POS.x = BORDER_AREA.x2 + 1;
    }

    // Movimenta o player para a direita ou esquerda
    if ((PRESSED_KEY == KEY_RIGHT) && (PLAYER_POS.x + 3 < BORDER_AREA.x2)) {
        PLAYER_POS.x += RIGHT;
    } else if ((PRESSED_KEY == KEY_LEFT) && (PLAYER_POS.x - 1 > BORDER_AREA.x1)) {
        PLAYER_POS.x += LEFT;
    }
}

void aliensMovement() {
    // Variaveis de loop
    int i, j;

    // Movimentacao dos aliens
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            if (ALIEN_DIRECTION == LEFT || ALIEN_DIRECTION == RIGHT) {
                ALIENS_POS[i][j].x += ALIEN_DIRECTION;
            } else if (ALIEN_DIRECTION == UP || ALIEN_DIRECTION == DOWN) {
                ALIENS_POS[i][j].y += ALIEN_DIRECTION / 2; // como up e down eh -2 ou 2, aqui pode ser -1 ou 1
            }
        }
    }

    // Calcula a direcao que os aliens devem ir no proximo passo
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            //Verifica se os aliens estao no mesmo y do player
            if (ALIENS_POS[i][j].y + 1 == PLAYER_POS.y) {
                gameOver(FALSE); // Fim de jogo - Derrota
                break;
            }
            // Colisao com a borda direita
            if (ALIENS_POS[i][j].x + 4 > BORDER_AREA.x2) {
                if (ALIEN_DIRECTION != DOWN) {
                    ALIEN_DIRECTION = DOWN;
                } else {
                    ALIEN_DIRECTION = LEFT;
                }
                break;
            }
                // Colisao com a borda esquerda
            else if (ALIENS_POS[i][j].x - 2 < BORDER_AREA.x1) {
                if (ALIEN_DIRECTION != DOWN) {
                    ALIEN_DIRECTION = DOWN;
                } else {
                    ALIEN_DIRECTION = RIGHT;
                }
                break;
            }
        }
    }
}

void lasersMovement() {
    // Movimenta o laser para cima se esta dentro das bordas
    if (LASER_POS[0].y > BORDER_AREA.y1) {
        if (GLOBALTIME % 5 == 0) {
            LASER_POS[0].y -= 1;
        }
    } else {
        // Laser volta em cima do player
        LASER_POS[0].y = PLAYER_POS.y - 1;
        LASER_POS[0].x = PLAYER_POS.x + 1;
        IS_PLAYER_SHOOTING = FALSE;
    }
}

void aliensShoot() {
    int j = 4; // Ultima fileira de aliens
    int alien_shooting;

    // Define qual alien vai disparar, randomicamente
    srand((unsigned) time(NULL));
    alien_shooting = rand() % 5;

    // Movimenta o laser para baixo se esta dentro das bordas
    if (LASER_POS[1].y <= PLAYER_POS.y) {
        LASER_POS[1].y += 1;

        // Caso o laser acerte o player -> Fim de jogo - Derrota
        if ((LASER_POS[1].y == PLAYER_POS.y + 1) && (LASER_POS[1].x == PLAYER_POS.x) ||
            (LASER_POS[1].y == PLAYER_POS.y) && (LASER_POS[1].x == PLAYER_POS.x + 1) ||
            (LASER_POS[1].y == PLAYER_POS.y + 1) && (LASER_POS[1].x == PLAYER_POS.x + 2)) {
            gameOver(FALSE);
        }
    } else {
        // Laser volta embaixo dos aliens
        LASER_POS[1].y = ALIENS_POS[alien_shooting][j].y + 2;
        LASER_POS[1].x = ALIENS_POS[alien_shooting][j].x;
    }

    // TODO: verificar aliens atingidos
}

void playerShoot() {
    // Atire se o player apertar espaco e nao houver outro disparo em andamento
    if ((PRESSED_KEY == ' ') && (IS_PLAYER_SHOOTING == FALSE)) {
        IS_PLAYER_SHOOTING = TRUE;
        // Define o x do laser apenas uma vez, corrige o bug de desvio do tiro
        LASER_POS[0].x = PLAYER_POS.x + 1;
    }

    if (IS_PLAYER_SHOOTING) {
        lasersMovement();
    }
}

void gameOver(bool winner) {
    // Encerra o jogo
    GAME_STATUS = FALSE;

    // Posicao da mensagem
    vec2 gmOverMsgPos;
    gmOverMsgPos.x = BORDER_AREA.x1 + 5;
    gmOverMsgPos.y = BORDER_AREA.y1 + 2;

    // Direcao do movimento da mensagem
    int xMsgDir = RIGHT;
    int yMsgDir = DOWN;

    // Enquanto a tecla q nao for apertada...
    while (PRESSED_KEY != 'q') {
        // Pega qual tecla esta sendo pressionada no momento
        getPressedKey();

        // Limpa o console
        clear();

        // Debug
#ifdef DEBUG
        drawDebug();
#endif

        // Desenha a borda
        drawBorder();

        // Desenha a mensagem de gameover
        mvprintw(gmOverMsgPos.y - 1, gmOverMsgPos.x, "   You %s with %d Points! Press q to exit!",
                 winner ? "won" : "lost", SCORE);
        mvprintw(gmOverMsgPos.y, gmOverMsgPos.x, "   ___   _   __  __ ___    _____   _____ ___");
        mvprintw(gmOverMsgPos.y + 1, gmOverMsgPos.x, "  / __| /_\\ |  \\/  | __|  / _ \\ \\ / / __| _ \\");
        mvprintw(gmOverMsgPos.y + 2, gmOverMsgPos.x, " | (_ |/ _ \\| |\\/| | _|  | (_) \\ V /| _||   /");
        mvprintw(gmOverMsgPos.y + 3, gmOverMsgPos.x, "  \\___/_/ \\_\\_|  |_|___|  \\___/ \\_/ |___|_|_\\");

        // Movimenta a mensagem de gameover, verificando colisoes
        if (GLOBALTIME % 25 == 0) {
            // Movimenta a msg com a direcao
            gmOverMsgPos.x += xMsgDir;
            gmOverMsgPos.y += yMsgDir / 2;

            // Colisao no eixo x
            if (gmOverMsgPos.x + 45 >= BORDER_AREA.x2) {
                xMsgDir = LEFT;
            } else if (gmOverMsgPos.x - 1 <= BORDER_AREA.x1) {
                xMsgDir = RIGHT;
            }

            // Colisao no eixo y
            if (gmOverMsgPos.y + 4 >= BORDER_AREA.y2) {
                yMsgDir = UP;
            } else if (gmOverMsgPos.y - 2 <= BORDER_AREA.y1) {
                yMsgDir = DOWN;
            }
        }

        // Refresh do ncurses
        refresh();

        // Incrementa o GLOBALTIME para animar a mensagem acima
        GLOBALTIME++;

        // Delay de espera
        usleep(DELAY);
    }

    // Manda um '^C' simulado pro programa, sinal de interrupcao
    kill(getpid(), SIGINT);

    // endwin() e reset_shell_mode() nao funcionam da maneira correta, quebram o console.
}
