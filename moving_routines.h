#include "restarting_routines.h"

void update_body(t_item *alien, int pos, int flip);
int move_right(t_list *mob, int flip, int *max_row);
int move_left(t_list *mob, int flip, int *max_row);
void switch_directions(t_list *mob);
int move_mob(t_game *game, int enabler);

void update_shots(t_list *shots);
void update_bombs(t_list *bombs);

void move_mothership(t_item *mothership);
void move_tank(t_game *game);