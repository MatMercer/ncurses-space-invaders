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

    // Inicia os players
    initPlayer();

    // Inicia os aliens
    initAliens();
}

void initPlayer(){
    // O player comeca no meio
    PLAYER_POS.x = WIN_SIZE.x / 2;
}

void initAliens() {
    // Variaveis de loop
    int i, j;

    // Aloca os aliens
    ALIENS_POS = calloc(ALIENS_ROWS, sizeof(vec2*));
    for(i = 0; i < ALIENS_ROWS; i++) {
        ALIENS_POS[i] = calloc(ALIENS_COLUMNS, sizeof(vec2));
    }

    // Coloca os aliens na posicao inicial
    for(i = 0; i < ALIENS_ROWS; i++) {
        for(j = 0; j < ALIENS_COLUMNS; j++) {
            ALIENS_POS[i][j].x = 1 + BORDER_AREA.x1 + (3 * i);
            ALIENS_POS[i][j].y = 1 + BORDER_AREA.y1 + (2 * j);
        }
    }

    // Manda eles andarem para a direita
    ALIEN_DIRECTION = RIGHT;
}

void play() {
    // Enquanto nao for gameover...
    while(GAME_STATUS) {
        // Pega qual tecla esta sendo pressionada
        getPressedKey();

        // Movimenta o jogo
        playerMovement();
        if(GLOBALTIME % 5 == 0)
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

    // Contagem de Pontos
    mvprintw(BORDER_AREA.y1 - 2, BORDER_AREA.x1, "SCORE: %u", SCORE);

    // Tamanho da tela recalculado a cada loop (responsivo)
    // Por enquanto desativado, quebra o jogo
    // getWinSize();

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

    // Verifica se a WIN_SIZE nao esta menor que o tamanho minimo
    if(WIN_SIZE.x < MIN_WINX) {
        WIN_SIZE.x = MIN_WINX;
    }
    if(WIN_SIZE.y < MIN_WINY) {
        WIN_SIZE.y = MIN_WINY;
    }
}

void drawBorder() {
    int i;

    // Tamanho da area da borda
    BORDER_AREA.x1 = WIN_SIZE.x/2 - WIN_SIZE.x/3;
    BORDER_AREA.x2 = WIN_SIZE.x/2 + WIN_SIZE.x/3;
    BORDER_AREA.y1 = WIN_SIZE.y/2 - WIN_SIZE.y/3;
    BORDER_AREA.y2 = WIN_SIZE.y/2 + WIN_SIZE.y/3;

    // Bordas Verticais
    for(i = BORDER_AREA.y1; i < BORDER_AREA.y2; i++) {
        mvprintw(i, BORDER_AREA.x1, BORDA);
        mvprintw(i, BORDER_AREA.x2, BORDA);
    }

    // Bordas Horizontais
    for(i = BORDER_AREA.x1; i <= BORDER_AREA.x2; i++) {
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

    for(i = 0; i < ALIENS_ROWS; i++) {
        for(j = 0; j < ALIENS_COLUMNS; j++) {
            mvprintw(ALIENS_POS[i][j].y, ALIENS_POS[i][j].x, "@ @");
            mvprintw(ALIENS_POS[i][j].y + 1, ALIENS_POS[i][j].x, " @");
        }
    }
}

void playerMovement() {
    // Manda o player sempre para a parte de baixo da borda
    PLAYER_POS.y = BORDER_AREA.y2 - 2;

    // Verifica se o player nao esta fora da borda
    if(PLAYER_POS.x + 4 > BORDER_AREA.x2) {
        PLAYER_POS.x = BORDER_AREA.x2 - 3;
    }
    else if(PLAYER_POS.x < BORDER_AREA.x1) {
        PLAYER_POS.x = BORDER_AREA.x2 + 1;
    }

    // Movimenta o player para a direita ou esquerda
    if((PRESSED_KEY == KEY_RIGHT) && (PLAYER_POS.x + 3 < BORDER_AREA.x2)) {
        PLAYER_POS.x += 1;
    }
    else if((PRESSED_KEY == KEY_LEFT) && (PLAYER_POS.x - 1 > BORDER_AREA.x1)) {
        PLAYER_POS.x -= 1;
    }
}

void aliensMovement() {
    // Variaveis de loop
    int i, j;

    // Movimentacao dos aliens
    for(i = 0; i < ALIENS_ROWS; i++) {
        for(j = 0; j < ALIENS_COLUMNS; j++) {
            if(ALIEN_DIRECTION == LEFT || ALIEN_DIRECTION == RIGHT) {
                ALIENS_POS[i][j].x += ALIEN_DIRECTION;
            }
            else if(ALIEN_DIRECTION == UP || ALIEN_DIRECTION == DOWN) {
                ALIENS_POS[i][j].y += ALIEN_DIRECTION / 2; // como up e down eh -2 ou 2, aqui pode ser -1 ou 1
            }
        }
    }

    // Calcula a direcao que os aliens devem ir no proximo passo
    for(i = 0; i < ALIENS_ROWS; i++) {
        for(j = 0; j < ALIENS_COLUMNS; j++) {
            //Verifica se os aliens estao no mesmo y do player
            if(ALIENS_POS[i][j].y + 1 == PLAYER_POS.y){
                gameOver(FALSE); // Fim de jogo - Derrota
                break;
            }
            // Colisao com a borda direita
            if(ALIENS_POS[i][j].x + 4 > BORDER_AREA.x2) {
                if(ALIEN_DIRECTION != DOWN) {
                    ALIEN_DIRECTION = DOWN;
                }
                else {
                    ALIEN_DIRECTION = LEFT;
                }
                break;
            }
            // Colisao com a borda esquerda
            else if(ALIENS_POS[i][j].x - 2 < BORDER_AREA.x1) {
                if(ALIEN_DIRECTION != DOWN) {
                    ALIEN_DIRECTION = DOWN;
                }
                else {
                    ALIEN_DIRECTION = RIGHT;
                }
                break;
            }
        }
    }
}

void gameOver(bool winner) {
    // Encerra o jogo
    GAME_STATUS = FALSE;

    //BUGS: printf funciona mas mvprintw nao, terminal aparece no meio do jogo

    // Posicao da mensagem
    vec2 gmOverMsgPos;
    gmOverMsgPos.x = BORDER_AREA.x1 + 5;
    gmOverMsgPos.y = BORDER_AREA.y1 + 2;

    // Direcao do movimento da mensagem
    int xMsgDir = RIGHT;
    int yMsgDir = DOWN;

    // Enquanto a tecla q nao for apertada...
    while(PRESSED_KEY != 'q') {
        // Pega qual tecla esta sendo pressionada no momento
        getPressedKey();

        // Limpa o console
        clear();

        // Desenha a borda
        drawBorder();

        // Desenha a mensagem de gameover
        mvprintw(gmOverMsgPos.y - 1, gmOverMsgPos.x, "You %s with %d Points! Press q to exit!", winner ? "won":"lost", SCORE);
        mvprintw(gmOverMsgPos.y, gmOverMsgPos.x,      "   ___   _   __  __ ___    _____   _____ ___");
        mvprintw(gmOverMsgPos.y + 1, gmOverMsgPos.x,  "  / __| /_\\ |  \\/  | __|  / _ \\ \\ / / __| _ \\");
        mvprintw(gmOverMsgPos.y + 2, gmOverMsgPos.x,  " | (_ |/ _ \\| |\\/| | _|  | (_) \\ V /| _||   /");
        mvprintw(gmOverMsgPos.y + 3, gmOverMsgPos.x,  "  \\___/_/ \\_\\_|  |_|___|  \\___/ \\_/ |___|_|_\\");

        // Movimenta a mensagem de gameover, verificando colisoes
        if(GLOBALTIME % 25 == 0) {
            // Movimenta a msg com a direcao
            gmOverMsgPos.x += xMsgDir;
            gmOverMsgPos.y += yMsgDir/2;

            // Colisao no eixo x
            if(gmOverMsgPos.x + 45 >= BORDER_AREA.x2) {
                xMsgDir = LEFT;
            }
            else if(gmOverMsgPos.x - 1 <= BORDER_AREA.x1) {
                xMsgDir = RIGHT;
            }

            // Colisao no eixo y
            if(gmOverMsgPos.y + 4 >= BORDER_AREA.y2) {
                yMsgDir = UP;
            }
            else if(gmOverMsgPos.y - 2 <= BORDER_AREA.y1) {
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

    // Resete a configuracao da shell para a anterior
    //reset_shell_mode();
    //endwin(); // endwin e reset_shell_mode nao funcionam da maneira correta, quebram o console.
}
