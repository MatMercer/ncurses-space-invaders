#include "spcinv.h"


static void initGame() {
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

    // Pontuacao Inicial
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

static void initPlayer() {
    // O player comeca no meio e embaixo
    PLAYER_POS.x = WIN_SIZE.x / 2;
    PLAYER_POS.y = BORDER_AREA.y2 - 2;
    PLAYER_LIVES = 3;
}

static void initAliens() {
    // Variaveis de loop
    int i, j;

    // Aloca os aliens
    ALIENS = calloc(ALIENS_ROWS, sizeof(component *));
    for (i = 0; i < ALIENS_ROWS; i++) {
        ALIENS[i] = calloc(ALIENS_COLUMNS, sizeof(component));
    }

    // Coloca os aliens na posicao inicial
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            ALIENS[i][j].pos.x = 1 + BORDER_AREA.x1 + (3 * i);
            ALIENS[i][j].pos.y = 1 + BORDER_AREA.y1 + (2 * j);
            // Estao vivos
            ALIENS[i][j].isAlive = TRUE;
        }
    }

    // Manda eles andarem para a direita
    ALIENS_DIRECTION = RIGHT;
}

static void initLasers() {
    // Laser[0] comeca 'em cima' do player
    LASER_POS[0].x = PLAYER_POS.x + 1;
    LASER_POS[0].y = PLAYER_POS.y + 1;

    // Posicoes Iniciais
    LASER_POS[1].y = ALIENS[0][ALIENS_COLUMNS - 1].pos.y + 11;
    LASER_POS[1].x = ALIENS[0][ALIENS_COLUMNS - 1].pos.x + 1;
    LASER_POS[2].y = ALIENS[ALIENS_ROWS / 2][ALIENS_COLUMNS - 1].pos.y + 6;
    LASER_POS[2].x = ALIENS[ALIENS_ROWS / 2][ALIENS_COLUMNS - 1].pos.x + 1;
    LASER_POS[3].y = ALIENS[ALIENS_ROWS - 1][ALIENS_COLUMNS - 1].pos.y + 1;
    LASER_POS[3].x = ALIENS[ALIENS_ROWS - 1][ALIENS_COLUMNS - 1].pos.x + 1;
}

extern void startSpaceInvaders() {
    initGame();
    // Enquanto nao for gameover...
    while (GAME_STATUS) {

        // Renderiza
        render();

        // Pega qual tecla esta sendo pressionada
        getPressedKey();

        // Movimenta o jogo
        playerMovement();
        playerShoot();

        if (GLOBALTIME % 8 == 0) {
            aliensMovement();
            aliensShoot();
        }

        // Incrementa o GLOBALTIME
        GLOBALTIME++;

        // Detecta colisoes aliens vs player ou laser de alien vs player
        playerLife();

        // Detecta colisoes dos aliens com o laser do player
        aliensLife();

        // Tempo de 'recarga'
        usleep(DELAY);
    }
}

static void render() {
    // Limpa o console
    clear();

    // Contagem de Pontos
    mvprintw(BORDER_AREA.y1 - 2, BORDER_AREA.x1, "LIVES: %u", PLAYER_LIVES);
    mvprintw(BORDER_AREA.y1 -2, BORDER_AREA.x2 - 9, "SCORE: %u", SCORE);

#ifdef DEBUG
    // Debug
    drawDebug();
#endif

    // Renderiza os objetos do jogo
    drawBorder();
    drawPlayer();
    drawAliens();
    drawLasers();

    // Manda para o console
    refresh();
}

static void getPressedKey() {
    PRESSED_KEY = getch();
}

static void getWinSize() {
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

static void drawBorder() {
    int i;

    // Tamanho da area da borda
    BORDER_AREA.x1 = WIN_SIZE.x / 2 - WIN_SIZE.x / 3;
    BORDER_AREA.x2 = WIN_SIZE.x / 2 + WIN_SIZE.x / 3;
    BORDER_AREA.y1 = WIN_SIZE.y / 2 - WIN_SIZE.y / 3;
    BORDER_AREA.y2 = WIN_SIZE.y / 2 + WIN_SIZE.y / 3;

    // Bordas Verticais
    for (i = BORDER_AREA.y1; i < BORDER_AREA.y2; i++) {
        mvprintw(i, BORDER_AREA.x1, BORDER);
        mvprintw(i, BORDER_AREA.x2, BORDER);
    }

    // Bordas Horizontais
    for (i = BORDER_AREA.x1; i <= BORDER_AREA.x2; i++) {
        mvprintw(BORDER_AREA.y1, i, BORDER);
        mvprintw(BORDER_AREA.y2, i, BORDER);
    }
}

static void drawPlayer() {
    mvprintw(PLAYER_POS.y, PLAYER_POS.x, " @");
    mvprintw(PLAYER_POS.y + 1, PLAYER_POS.x, "@@@");
}

static void drawAliens() {
    int i, j;

    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            if (ALIENS[i][j].isAlive) {
                mvprintw(ALIENS[i][j].pos.y, ALIENS[i][j].pos.x, "@ @");
                mvprintw(ALIENS[i][j].pos.y + 1, ALIENS[i][j].pos.x, " @");
            }
        }
    }
}

static void drawLasers() {
    int laser_index;

    // Se nao estiver 'em cima' do player
    if (LASER_POS[0].y < PLAYER_POS.y) {
        // Desenha o laser disparado pelo player
        mvprintw(LASER_POS[0].y, LASER_POS[0].x, "|");
    }

    // Desenha o laser dos aliens
    for (laser_index = 1; laser_index < MAX_LASERS; laser_index++) {
        mvprintw(LASER_POS[laser_index].y, LASER_POS[laser_index].x, "|");
    }
}

static void moveComponent(component *comp) {
    if (comp->isAlive) {
        // += eh um macete para direcao no terminal
        if (comp->direction == RIGHT || comp->direction == LEFT) {
            comp->pos.x += comp->direction;
        } else if (comp->direction == UP || comp->direction == DOWN) {
            comp->pos.y += comp->direction / 2; // como UP eh -2, -2/2 == -1, decrementa o Y etc
        }
    }
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
        case 0:
            return "NONE";
        default:
            return "UNDEFINED";
    }
}

static void drawDebug() {
    mvprintw(BORDER_AREA.y1 - 8, BORDER_AREA.x1, "WINDOW SIZE: %d:%d\t\tBORDER AREA: %d:%d:%d:%d \tGLOBALTIME: %ld ",
             WIN_SIZE.x, WIN_SIZE.y, BORDER_AREA.x1, BORDER_AREA.x2, BORDER_AREA.y1, BORDER_AREA.y2, GLOBALTIME);
    mvprintw(BORDER_AREA.y1 - 6, BORDER_AREA.x1, "LAST ROW/LAST ALIEN: %u:%u\t\tPRESSED KEY: %d \t\tPLAYER POS: %d:%d",
             LAST_ALIVE_ROW, LAST_ALIVE_ALIEN, PRESSED_KEY, PLAYER_POS.x, PLAYER_POS.y);
    mvprintw(BORDER_AREA.y1 - 4, BORDER_AREA.x1, "ALIENS DIRECTION: %s\t\tLASER POS: %d:%d \t\tGAME STATUS: %d",
             dirToString(ALIENS_DIRECTION), LASER_POS[0].x, LASER_POS[0].y, GAME_STATUS);
}

#endif

static void playerMovement() {
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

static void aliensMovement() {
    // Variaveis de loop
    int i, j;

    // Movimentacao dos aliens
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            if (ALIENS[i][j].isAlive) {
                ALIENS[i][j].direction = ALIENS_DIRECTION;
                moveComponent(&ALIENS[i][j]);
            }
        }
    }

    // Calcula a direcao que os aliens devem ir no proximo passo
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {

            if (ALIENS[i][j].isAlive) {
                // Colisao com a borda direita
                if (ALIENS[i][j].pos.x + 4 > BORDER_AREA.x2) {
                    if (ALIENS_DIRECTION != DOWN) {
                        ALIENS_DIRECTION = DOWN;
                    } else {
                        ALIENS_DIRECTION = LEFT;
                    }
                    break;
                }
                // Colisao com a borda esquerda
                else if (ALIENS[i][j].pos.x - 2 < BORDER_AREA.x1) {
                    if (ALIENS_DIRECTION != DOWN) {
                        ALIENS_DIRECTION = DOWN;
                    } else {
                        ALIENS_DIRECTION = RIGHT;
                    }
                    break;
                }
            }
        }
    }
}

static void aliensLife() {
    // Variaveis de loop
    int i, j;

    // Verifica colisoes com laser do player
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            // Esquerda - Meio - Direita
            if (((LASER_POS[0].x == ALIENS[i][j].pos.x) && (LASER_POS[0].y == ALIENS[i][j].pos.y)) ||
                 ((LASER_POS[0].x == ALIENS[i][j].pos.x + 1) && (LASER_POS[0].y == ALIENS[i][j].pos.y + 1)) ||
                 ((LASER_POS[0].x == ALIENS[i][j].pos.x + 1) && (LASER_POS[0].y == ALIENS[i][j].pos.y)) ||
                 ((LASER_POS[0].x == ALIENS[i][j].pos.x + 2) && (LASER_POS[0].y == ALIENS[i][j].pos.y))) {

                if (ALIENS[i][j].isAlive) {
                    ALIENS[i][j].isAlive = FALSE;
                    IS_PLAYER_SHOOTING = FALSE;

                    // Incrementa o score em + 10 pontos
                    SCORE += KILL_SCORE;
                }

                // Se todos os X aliens foram atingidos -> Vitoria
                if (SCORE == (ALIENS_COLUMNS * ALIENS_ROWS * KILL_SCORE)) {
                    gameOver(TRUE);
                }
            }
        }
    }
}

static void playerLife() {
    // Variaveis de loop
    int i, j, laser_index;

    // Percorre os lasers disponives para os aliens
    for (laser_index = 1; laser_index < MAX_LASERS; laser_index++) {

        // Detecta colisões com tiros em todas as partes "renderizaveis" do player
        if ((LASER_POS[laser_index].y == PLAYER_POS.y && LASER_POS[laser_index].x == PLAYER_POS.x + 1) ||       // Cima
            (LASER_POS[laser_index].y == PLAYER_POS.y + 1 && LASER_POS[laser_index].x == PLAYER_POS.x) ||       // Baixo Esquerda
            (LASER_POS[laser_index].y == PLAYER_POS.y + 1 && LASER_POS[laser_index].x == PLAYER_POS.x + 1) ||   // Baixo Meio
            (LASER_POS[laser_index].y == PLAYER_POS.y + 1 && LASER_POS[laser_index].x == PLAYER_POS.x + 2)) {   // Baixo Direita
            // Player perde uma vida, se nao restar nenhuma -> gameOver
            PLAYER_LIVES -= 1;

            // Remove o laser colidido, evitando que o player perca mais de uma vida de uma vez
            LASER_POS[laser_index].x = -1;
            LASER_POS[laser_index].y = -1;
            break;
        }
    }

    if (PLAYER_LIVES == 0) {
        gameOver(FALSE);
    }

    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {

            if (ALIENS[i][j].isAlive) {
                // Verifica se os aliens estao colidindo com o player ou com a borda inferior
                if ((ALIENS[i][j].pos.y == BORDER_AREA.y2 - 2) ||
                    ((ALIENS[i][j].pos.y + 1 == PLAYER_POS.y) && (ALIENS[i][j].pos.x == PLAYER_POS.x + 1)) ||       // Cima
                    ((ALIENS[i][j].pos.y + 1 == PLAYER_POS.y + 1) && (ALIENS[i][j].pos.x == PLAYER_POS.x)) ||       // Baixo Direita
                    ((ALIENS[i][j].pos.y + 1 == PLAYER_POS.y + 1) && (ALIENS[i][j].pos.x == PLAYER_POS.x + 2)) ) {  // Baixo Esquerda
                    gameOver(FALSE); // Fim de jogo - Derrota
                    break;
                }
            }
        }
    }
}

static void playerLaser() {
    // Movimenta o laser para cima se esta dentro das bordas
    if (LASER_POS[0].y > BORDER_AREA.y1) {
        if (GLOBALTIME % 5 == 0) {
            LASER_POS[0].y -= 1;
        }
    } else {
        // Laser volta 'em cima' do player
        LASER_POS[0].y = PLAYER_POS.y + 1;
        LASER_POS[0].x = PLAYER_POS.x + 1;
        IS_PLAYER_SHOOTING = FALSE;
    }
}

static void aliensShoot() {
    int i, j, laser_index; // Variaveis de loop
    int rand_col;
    int aliens_index[ALIENS_ROWS];

    // Gera varios rands em um curto intervalo de tempo
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand((unsigned int) (t1.tv_usec * t1.tv_sec));

    // Verifica qual a fileira com aliens vivos mais proxima do player
    LAST_ALIVE_ROW = 0;
    for (i = 0; i < ALIENS_ROWS; i++) {
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            if (ALIENS[i][j].isAlive && j >= LAST_ALIVE_ROW) {
                LAST_ALIVE_ROW = (unsigned) j;
            }
        }
    }

    // Verifica qual o ultimo alien vivo da ultima fileira
    LAST_ALIVE_ALIEN = 0;
    for (i = 0; i < ALIENS_COLUMNS; i++) {
        if (ALIENS[i][LAST_ALIVE_ROW].isAlive && i >= LAST_ALIVE_ALIEN) {
            LAST_ALIVE_ALIEN = (unsigned) i;
        }
    }

    // Define o ultimo alien vivo de cada coluna
    for (i = 0; i < ALIENS_ROWS; i++) {
        aliens_index[i] = -1;
        for (j = 0; j < ALIENS_COLUMNS; j++) {
            if (ALIENS[i][j].isAlive) {
                aliens_index[i] = j;
            }
        }
    }

    // Define qual alien vai disparar, randomicamente
    rand_col = rand() % ALIENS_ROWS;

    // Percorre os lasers disponives para os aliens
    for (laser_index = 1; laser_index < MAX_LASERS; laser_index++) {

        // Movimenta o laser para baixo se esta dentro das bordas
        if ((LASER_POS[laser_index].y < BORDER_AREA.y2 - 1) &&
             (LASER_POS[laser_index].x > BORDER_AREA.x1)) {
            LASER_POS[laser_index].y += 1;
        } else if (ALIENS[rand_col][aliens_index[rand_col]].isAlive) {
            // Laser volta embaixo do alien vivo gerado pelo rand
            LASER_POS[laser_index].y = ALIENS[rand_col][aliens_index[rand_col]].pos.y + 1;
            LASER_POS[laser_index].x = ALIENS[rand_col][aliens_index[rand_col]].pos.x + 1;
        } else {
            // Se o alien gerado pelo rand estiver morto, mude para o ultimo vivo da ultima coluna
            LASER_POS[laser_index].y = ALIENS[LAST_ALIVE_ROW][LAST_ALIVE_ALIEN].pos.y + 1;
            LASER_POS[laser_index].x = ALIENS[LAST_ALIVE_ROW][LAST_ALIVE_ALIEN].pos.x + 1;
        }
    }
}

static void playerShoot() {
    if (IS_PLAYER_SHOOTING) {
        playerLaser();
    } else {
        // Laser volta 'em cima' do player se nao estiver subindo
        LASER_POS[0].y = PLAYER_POS.y + 1;
        LASER_POS[0].x = PLAYER_POS.x + 1;
    }

    // Atire se o player apertar espaco e nao houver outro disparo em andamento
    if ((PRESSED_KEY == ' ') && (IS_PLAYER_SHOOTING == FALSE)) {
        IS_PLAYER_SHOOTING = TRUE;
        // Define o x do laser apenas uma vez, corrige o bug de desvio do tiro
        LASER_POS[0].x = PLAYER_POS.x + 1;
    }
}

static void gameOver(bool winner) {
    // Encerra o jogo
    GAME_STATUS = FALSE;

    // Posicao da mensagem
    vec2 gmOverMsgPos;
    gmOverMsgPos.x = BORDER_AREA.x1 + 5;
    gmOverMsgPos.y = BORDER_AREA.y1 + 2;

    // Direcao do movimento da mensagem
    int xMsgDir = RIGHT;
    int yMsgDir = DOWN;

    // Enquanto a tecla 'q' nao for apertada...
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
        mvprintw(gmOverMsgPos.y - 1, gmOverMsgPos.x, "You %s! [%d Points] q to exit or r to reset!",
                 winner ? "won" : "lost", SCORE);
        mvprintw(gmOverMsgPos.y, gmOverMsgPos.x, "  ___   _   __  __ ___    _____   _____ ___");
        mvprintw(gmOverMsgPos.y + 1, gmOverMsgPos.x, " / __| /_\\ |  \\/  | __|  / _ \\ \\ / / __| _ \\");
        mvprintw(gmOverMsgPos.y + 2, gmOverMsgPos.x, "| (_ |/ _ \\| |\\/| | _|  | (_) \\ V /| _||   /");
        mvprintw(gmOverMsgPos.y + 3, gmOverMsgPos.x, " \\___/_/ \\_\\_|  |_|___|  \\___/ \\_/ |___|_|_\\");

        if (PRESSED_KEY == 'r' || PRESSED_KEY == 'R') {
            startSpaceInvaders();
        }

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
