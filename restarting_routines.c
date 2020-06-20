#define _DEFAULT_SOURCE
#include "restarting_routines.h"
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Utility swapping functions for the score sorting. */
void swap(int *in, int *out) {
	int aux=*in;
	*in = *out;
	*out = aux;
}

void swap_str(char *in, char *out) {
	char *aux;
    aux = (char*) malloc(sizeof(char)*strlen(in));
    strcpy(aux, out);
    strcpy(out, in);
    strcpy(in, aux);
    free(aux);
}

/* A generic selection sort algorithm for reverse order sorting. */
void sort_scores(char (*scores)[SCORE_LENGTH+1], char (*names)[NAME_LENGTH+1], int *v, int size) {
	int i, j, max;
	for (i=0; i!=size-1; i++) {
        max = i;
        for (j=i+1; j!=size; j++) {
            if (v[j] > v[max])
                max = j;
        }
        if (i!=max) {
            swap_str(scores[i], scores[max]);
            swap_str(names[i], names[max]);
            swap(&v[i], &v[max]);
        }
    }
}

void save_score(int score, char name[4]) {
	FILE *output;
	char names[N_SCORES+2][NAME_LENGTH+1], scores[N_SCORES+2][SCORE_LENGTH+1];
	int aux, score_aux[N_SCORES+1]; /* An extra space for the current player's score. */

	/* Save score of current player. */
	output = fopen("scores.txt", "a");
	fprintf(output, "%s %08d\n", name, score);
	fclose(output);

	/* Reading all scores. */
	output = fopen("scores.txt", "r");
	for (aux=0; aux!=N_SCORES+1; aux++) {
		fscanf(output, "%s %s", names[aux], scores[aux]);
		score_aux[aux] = atoi(scores[aux]);
	}
	fclose(output);

	/* Sort the scores in reverse order and print the first five to the scores file. */
	sort_scores(scores, names, score_aux, N_SCORES+1);
	output = fopen("scores.txt", "w");
	for (aux=0; aux!=N_SCORES; aux++)
        fprintf(output, "%s %s\n", names[aux], scores[aux]);
    fclose(output);
}

/* Endgame animation. */
void destroy_screen() {
	int i, j, k;
	for (i=0; i<=(ROW+10); i++) {
		if (i%10 == 0)
			wattron(stdscr, COLOR_PAIR((rand()%7)+1));
		if (i <= ROW)
			for (j=0; j<=11; j++) {
				mvprintw(i, j*9, "YOU LOSE ");
			}
		if (i >= 10) {
			for (j=0; j<=11; j++)
				for (k=0; k<=8; k++)
					mvaddch(i-10, j*9+k, ' ');
		}
		refresh();
		usleep(50000);
	}
}

/* Destroy the game. */
void endgame(t_game *game) {
	destroy_list(&game->mob);
    destroy_list(&game->barriers);
    destroy_list(&game->shots);
    destroy_list(&game->bombs);
    free(game->player.body);

	if (game->state == DEAD) {
		save_score(game->score, game->name);
		destroy_screen();
		wattron(stdscr, COLOR_PAIR(BOX));
		nodelay(stdscr, FALSE);
		mvprintw(ROW/2 - 1, (COL/2)-8, "Play again? y/n");
		char c;
		c=getch();
		switch (c) {
			case 'y':
				initialize_game(game);
				return;
			break;
		}
	}
    endwin();
    exit(0);
}

void restart_screen() {
	int i=0, j;
	wattron(stdscr, COLOR_PAIR(BOX));
	for (j=0; j<=12; j++)
			mvprintw(i, j*8, "YOU WIN ");
	refresh();
	for (i=1; i<=ROW; i++) {
		for (j=0; j<=12; j++)
			mvprintw(i, j*8, "YOU WIN ");
		for (j=0; j<=12; j++)
			mvprintw(i-1, j*8, "        ");
		refresh();
		usleep(50000);
	}
}

/* Recomeça o jogo. */
void restart(t_game *game) {
	restart_screen();

    /* Destroi as listas para livrar o espaço e deixar inicializar de novo. */
    destroy_list(&game->mob);
    destroy_list(&game->barriers);
    destroy_list(&game->shots);
    destroy_list(&game->bombs);
    free(game->player.body);

    /* Inicializa as entidades conforme função inicializa jogo. */
    initialize_list(&game->mob);
    initialize_list(&game->barriers);
    initialize_list(&game->shots);
    initialize_list(&game->bombs);

    initialize_barriers(&game->barriers);
    initialize_mob(&game->mob);
    initialize_tank(&game->player);

    /* Aspectos do novo jogo. */
    game->alive_enemies = MOB;
    game->n_wins++;
    game->delay = game->delay + 5;
    set_delay(game->delay);
    wattron(stdscr, COLOR_PAIR(game->n_wins%7+1));

    int aux;
    game->cycle = CYCLE;
    for (aux=0; aux!=game->n_wins; aux++)
    	game->cycle = game->cycle * 0.7;
}
