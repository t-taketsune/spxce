#include "printing_routines.h"
#include <ncurses.h>

/* As funções de impressão diferem pois o tamanho de cada item é diferente. */
void print_barrier(t_item barrier) {
    int i, j;
    for (i=0; i!=3; i++)
        for (j=0; j!=7; j++)
            mvaddch(barrier.row+i, barrier.col+j, barrier.body[j+(i*7)]);
}

void print_alien(t_item *alien) {
    int i, j;
    for (i=0; i!=3; i++)
        for (j=0; j!=3; j++)
            mvaddch(alien->row+i, alien->col+j, alien->body[j+(i*3)]);
    if (alien->state == DYING)
        alien->state = DEAD;
}

void print_tank(tank t) {
    int i, j;
    for (i=0; i!=2; i++)
        for (j=0; j!=5; j++)
            mvaddch(t.row+i, t.col+j, t.body[j+(i*5)]);
}

void print_mothership(t_item mothership) {
    int i, j;
    for (i=0; i!=3; i++) {
        for (j=0; j!=7; j++)
            mvaddch(mothership.row+i, mothership.col+j, mothership.body[j+(i*7)]);
    }
}

void print_barriers(t_list *barriers) {
    int aux;
    t_item barrier;
    initialize_current_first(barriers);
    for (aux=0; aux!=N_BAR; aux++) {
        check_current_item(&barrier, barriers);
        print_barrier(barrier);
        increment_current(barriers);
    }
}

/* Printa as bombas na tela. */
void print_bombs(t_list *bombs) {
    if (empty_list(bombs))
        return;
    int aux, size;
    t_item bomb;
    size_list(&size, bombs);
    initialize_current_first(bombs);
    for (aux=0; aux!=size; aux++) {
        check_current_item(&bomb, bombs);
        mvaddch(bomb.row, bomb.col, bomb.body[0]);
        increment_current(bombs);
    }
}

/* Imprime os tiros na tela. */
void print_shots(t_list *shots) {
    if (empty_list(shots))
        return;
    int aux, size;
    t_item shot;
    size_list(&size, shots);
    initialize_current_first(shots);
    for (aux=0; aux!=size; aux++) {
        check_current_item(&shot, shots);
        mvaddch(shot.row, shot.col, shot.body[0]);
        increment_current(shots);
    }
}

/* Imprime a onda de inimigos. */
void print_mob(t_list *mob) {
    int aux;
    t_item alien;
    initialize_current_first(mob);
    for (aux=0; aux!=MOB; aux++) {
        check_current_item(&alien, mob);
        if (alien.state != DEAD)
            print_alien(&alien);
        increment_current(mob);
    }
}

/* Desenha a caixa ao redor da interface e a pontuacao. */
void drawbox(int score, int stage) {
    int i;
    move(0, 0);
    addch('+');
    move(0, COL);
    addch('+');
    move(ROW, COL);
    addch('+');
    move(ROW, 0);
    addch('+');
    for (i=1; i!=ROW; i++) {
        move(i, 0);
        addch('|');
        move(i, COL);
        addch('|');
    }
    for (i=1; i!=COL; i++) {
        move(0, i);
        addch('-');
        move(ROW, i);
        addch('-');
    }
    move(1, (COL/2)-12);
    printw("%08d // STAGE: %d", score, stage);
}