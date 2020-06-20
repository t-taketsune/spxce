#include "list_space.h"

int barrier_collision(t_list *barriers, t_item *missile);
int alien_collision(t_list *mob, t_item *shot);
int player_collision(tank *player, t_item *bomb);
int barrier_alien_collision(t_item *barrier, t_item *alien);

int check_shots(t_list *shots, t_list *barriers, t_list *mob, int *enemies);
int check_bombs(t_list *bombs, t_list *barriers, tank *player);
int check_missiles(t_list *bombs, t_list *shots);
void check_barriers(t_list *barriers, t_list *mob);
int mothership_kill(t_list *shots, t_item *mothership);

void check_collisions(t_game *game, int max_row);