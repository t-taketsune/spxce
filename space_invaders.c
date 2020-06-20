#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "moving_routines.h"

int main() {
    t_game game;
    initialize_game(&game);
    int aux, max_row=0;
    while (TRUE) {
        for (aux=1; aux<=24; aux++) {
            erase();
            drawbox(game.score, game.n_wins+1);
            move_tank(&game);
            print_barriers(&game.barriers);
            print_tank(game.player);
            if (aux%4 == 0) {
                max_row = move_mob(&game, (aux%12==0));
            }
            if (aux%2 == 0) {
            	update_bombs(&game.bombs);
            }
            if (game.mothership.state == ALIVE) {
                print_mothership(game.mothership);
                if (aux%2 == 0) {
                    move_mothership(&game.mothership);
                    if (aux == 12)
                        release_bomb(game.mothership, &game.bombs);
                }
            }
            update_shots(&game.shots);
            print_mob(&game.mob);
            print_shots(&game.shots);
            print_bombs(&game.bombs);
            check_collisions(&game, max_row);
            if (game.alive_enemies == 0) {
            	game.state = WON;
                restart(&game);
            }
            if (max_row == ROW - 4)
            	game.state = DEAD;
            if (game.state == DEAD)
            	endgame(&game);
            refresh();
            usleep(game.cycle/6);
        }
    }
    endwin();
    return 1;
}
