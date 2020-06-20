#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include "initialization_routines.h"

/* Inicializa tanque com as configuracoes padroes.
   Note que alocamos um espaço a mais para a string do corpo, visto que a funcao strlen
   não inclui o caracter terminador. */
void initialize_tank(tank *player) {
    player->row = ROW-2;
    player->col = COL/2;
    player->type = TANK;
    player->dir = 0;
    player->body = (char*) malloc((strlen(TANK_BODY)+1)*sizeof(char));
    strcpy(player->body, TANK_BODY);
}

void initialize_barriers(t_list *barriers) {
    int aux;
    t_item input;

    /* Constantes para todas as barreiras definidas abaixo. */
    input.dir = NONE;
    input.row = ROW - 7;
    input.state = ALIVE;
    input.type = BARRIER;
    input.body = (char*) malloc((strlen(BARRIER_BODY)+1)*sizeof(char));
    strcpy(input.body, BARRIER_BODY);

    /* Adiciona na lista. */
    for (aux=0; aux!=N_BAR; aux++) {
        input.col = 20*(aux+1) - 4;
        insert_end_list(input, barriers);
    }
    free(input.body);
}

void initialize_mob(t_list *mob) {
    int aux, aux2;
    t_item input;

    /* Constantes que valem pra primeira mob inteira. O tamanho de todo alien e constante. */
    input.dir = RIGHT;
    input.state = ALIVE;
    input.type = ALIEN;
    input.body = (char*) malloc((strlen(ALIEN_1_LEFT)+1)*sizeof(char));

    /* Cada rodada do for interno cria 11 aliens na fileira indicada pelo for externo. */
    /* Fileiras de aliens começam na linha 6 para dar espaço para a nave mae, e tem um
       espaço em branco ao final para deixar espacado. */
    input.row = 6;
    for (aux2=1; aux2<=5; aux2++) {
        if (aux2 == 1)
            strcpy(input.body, ALIEN_1_LEFT);
        else if (aux2 == 2 || aux2 == 3)
            strcpy(input.body, ALIEN_2_LEFT);
        else
            strcpy(input.body, ALIEN_3_LEFT);
        for (aux=0; aux!=11; aux++) {
            input.col = 6*(aux+1) - 5;
            insert_end_list(input, mob);
        }
        input.row = input.row+4;
    }
    free(input.body);
}

int create_mothership(t_item *mothership) {
    mothership->row = 2;
    mothership->col = 1;
    mothership->dir = RIGHT;
    mothership->state = DEAD;
    mothership->type = MOTHERSHIP;
    mothership->body = (char*) malloc((strlen(MOTHERSHIP_BODY)+1)*sizeof(char));
    if (!mothership->body)
        return 0;
    strcpy(mothership->body, MOTHERSHIP_BODY);
    return 1;
}

void initial_screen(char name[4]) {
	wattron(stdscr, COLOR_PAIR(SHOT));
	mvprintw(ROW/2-1, COL/2-15, "WELCOME TO ASCII SPACE INVADERS");
	mvprintw(ROW/2, COL/2-10, "ENTER YOUR NAME BELOW");
	refresh();
	int i;
	move(ROW, COL/2-1);
	for (i=0; i!=3; i++)
		name[i] = (char) getch();
	getch();
}

void init_colors() {
    init_pair(TANK, COLOR_BLUE, COLOR_BLACK);
    init_pair(BARRIER, COLOR_GREEN, COLOR_BLACK);
    init_pair(ALIEN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SHOT, COLOR_CYAN, COLOR_BLACK);
    init_pair(BOMB, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(BOX, COLOR_WHITE, COLOR_BLACK);
    init_pair(MOTHERSHIP, COLOR_RED, COLOR_BLACK);
}

/* Inicia as configuracoes do ncurses. */
void init_curses(char *name) {
    initscr();
    start_color();
    int lin, col;
    getmaxyx(stdscr, lin, col);
    if (lin < 39 || col < 101) {
    	printf("The terminal must be at least 38 characters tall and 100 characters wide.");
    	endwin();
    	exit(0);
    }
    /* Inicia os pares de cores para cada elemento. */
    init_colors();
    initial_screen(name);
    wattron(stdscr, COLOR_PAIR(1));

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(FALSE);
}

void set_delay(int delay) {
	char *command;
    command = (char*) malloc(sizeof(char)*(strlen(XSET_COMMAND)+3));
    sprintf(command, "xset r rate 120 %d", delay);
    system(command);
    free(command);
}

void initialize_game(t_game *game) {
    /* Flip atua como variável booleana, com a mob assumindo uma das duas formas conforme seu valor. */
    game->alive_enemies = MOB;
    game->flip = 0;
    game->state = ALIVE;
    game->n_wins = 0;
    game->cycle = CYCLE;
    game->delay = 15;

    /* Inicializacao das listas contendo os TAD elementos do jogo. */
    initialize_list(&game->mob);
    initialize_list(&game->barriers);
    initialize_list(&game->shots);
    initialize_list(&game->bombs);

    /* Inicializacao das barreiras, aliens e tanque. */
    initialize_barriers(&game->barriers);
    initialize_mob(&game->mob);
    initialize_tank(&game->player);
    create_mothership(&game->mothership);

    /* Inicializacao da seed das bombas dos aliens. */
    time_t seed=time(NULL);
    srand((unsigned) time(&seed));

    /* Inicializa o terminal do curses. */
    init_curses(game->name);

    /* Inicializacao do delay. */
    set_delay(game->delay);
}