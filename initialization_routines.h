#include "shooting_routines.h"

void initialize_tank(tank *player);
void initialize_barriers(t_list *barriers);
void initialize_mob(t_list *mob);
int create_mothership(t_item *mothership);

void initial_screen(char name[4]);
void init_colors();
void init_curses(char *name);

void set_delay(int delay);
void initialize_game(t_game *game);