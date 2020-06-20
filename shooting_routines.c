#include "shooting_routines.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Faz o inimigo selecionado soltar uma bomba. Pode ser alien ou nave mãe. */
int release_bomb(t_item enemy, t_list *bombs) {
    t_item bomb;
    bomb.type = BOMB;
    bomb.dir = DOWN;
    bomb.state = ALIVE;
    bomb.row = enemy.row + 3;
    if (enemy.type == ALIEN)
        bomb.col = enemy.col + 1;
    else
        bomb.col = enemy.col + 4;
    bomb.body = (char*) malloc(sizeof(char)*2);
    if (!bomb.body)
    	return 0;
    strcpy(bomb.body, "$");
    insert_end_list(bomb, bombs);
    return 1;
}

/* Dropa bomba. Procura um alien aleatório, se este estiver vivo, dropa bomba. Se não, procura outro.
   Faz até cinco tentativas. */
int call_bomb(t_list *mob, t_list *bombs, t_item *alien) {
    int i=0;
    while (i!=5) {
        int shooter, aux;
        shooter = rand() % MOB;
        initialize_current_first(mob);
        for (aux=0; aux!=shooter; aux++)
            increment_current(mob);
        check_current_item(alien, mob);
        if (alien->state == ALIVE) {
            release_bomb(*alien, bombs);
            return 1;
        }
        i++;
    }
    return 0;
}

/* Verifica se a lista com os tiros tem mais do que o máximo de tiros, e, se não, cria mais um. */
int shoot(tank *player, t_list *shots) {
    int size;
    size_list(&size, shots);
    if (size >= MAX_SHOT)
        return 0;
    t_item shot;
    shot.row = player->row-1;
    shot.col = player->col+2;
    shot.type = SHOT;
    shot.state = ALIVE;
    shot.dir = UP;
    shot.body = malloc(sizeof(char)*2);
    if (!shot.body)
    	return 0;
    strcpy(shot.body, "|");
    insert_end_list(shot, shots);
    free(shot.body);
    return 1;
}