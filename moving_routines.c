#include "moving_routines.h"
#include <string.h>
#include <ncurses.h>

/* Atualiza corpo dos aliens, trocando entre as duas possibilidades conforme declarações de constantes. */
void update_body(t_item *alien, int pos, int flip) {
    if (flip == 1) {
        if (pos < 11)
            strcpy(alien->body, ALIEN_1_LEFT);
        else if (pos < 33)
            strcpy(alien->body, ALIEN_2_LEFT);
        else
            strcpy(alien->body, ALIEN_3_LEFT);
    }
    else {
        if (pos < 11)
            strcpy(alien->body, ALIEN_1_RIGHT);
        else if (pos < 33)
            strcpy(alien->body, ALIEN_2_RIGHT);
        else
            strcpy(alien->body, ALIEN_3_RIGHT);
    }
}

/* Move a onda de inimigos para a direita, e retorna a posição na coluna do inimigo mais a direita. */
int move_right(t_list *mob, int flip, int *max_row) {
    int aux, max_col=0;
    t_item alien;
    initialize_current_first(mob);
    for (aux=0; aux!=MOB; aux++) {
        check_current_item(&alien, mob);
        alien.col++;
        update_body(&alien, aux, flip);
        if (alien.row > *(max_row) && alien.state != DEAD)
            *(max_row) = alien.row;
        if (alien.col > max_col && alien.state != DEAD)
            max_col = alien.col;
        update_current_item(alien, mob);
        increment_current(mob);
    }
    return max_col;
}

/* Move a onda de inimigos a esquerda, e retorna a posição na coluna do inimigo mais a esquerda. */
int move_left(t_list *mob, int flip, int *max_row) {
    int aux, min_col=COL;
    t_item alien;
    initialize_current_first(mob);
    for (aux=0; aux!=MOB; aux++) {
        check_current_item(&alien, mob);
        alien.col--;
        update_body(&alien, aux, flip);
        if (alien.row > *(max_row) && alien.state != DEAD)
            *(max_row) = alien.row;
        if (alien.col < min_col && alien.state != DEAD)
            min_col = alien.col;
        update_current_item(alien, mob);
        increment_current(mob);
    }
    return min_col;
}

/* Troca a direção da onda de inimigos e desce uma linha. */
void switch_directions(t_list *mob) {
    int aux;
    t_item alien;
    initialize_current_first(mob);
    for (aux=0; aux!=MOB; aux++) {
        check_current_item(&alien, mob);
        if (alien.dir == LEFT)
            alien.dir = RIGHT;
        else
            alien.dir = LEFT;
        alien.row++;
        update_current_item(alien, mob);
        increment_current(mob);
    }
}

/* Atualiza as informacoes dos invasores. */
int move_mob(t_game *game, int enabler) {
    t_item alien;
    int max_row=0;
    initialize_current_first(&game->mob);
    check_current_item(&alien, &game->mob);

    /* Move a onda de acordo com a direção, e checa os extremos. */
    if (alien.dir == RIGHT) {
        int max_col;
        max_col=move_right(&game->mob, game->flip, &max_row);
        if (max_col >= COL-4)
             switch_directions(&game->mob);
    }
    else {
        int min_col;
        min_col=move_left(&game->mob, game->flip, &max_row);
        if (min_col <= 2)
            switch_directions(&game->mob);
    }

    /* Se a mob tiver descido muito, chama uma nave mãe. */
    if (max_row % 4 == 0 && game->mothership.state != DYING) {
        if (game->mothership.state == DEAD)	
        	game->cycle = game->cycle*0.8;
        game->mothership.state = ALIVE;
    }
    else if (max_row % 2 == 0 && max_row % 4 != 0)
    	game->mothership.state = DEAD;

    /* Faz um alien decidido aleatoriamente atirar, caso o ciclo tenha atingido a quarta
       rodada de movimento dos aliens.*/
    if (enabler) {
        call_bomb(&game->mob, &game->bombs, &alien);
    }

    /* Troca o flip. */
    if (game->flip == 1)
        game->flip = 0;
    else
        game->flip = 1;
    return max_row;
}

/* Sobe os tiros e os remove caso atinjam o topo da tela. */
void update_shots(t_list *shots) {
    if (empty_list(shots))
        return;
    int aux, size;
    t_item shot;
    size_list(&size, shots);
    initialize_current_first(shots);
    for (aux=0; aux!=size; aux++) {
        check_current_item(&shot, shots);
        shot.row--;
        if (shot.row == 1)
            remove_current_item(&shot, shots);
        else {
            update_current_item(shot, shots);
            increment_current(shots);
        }
    }
}

/* Desce as bombas e as remove caso atinjam o piso da tela. */
void update_bombs(t_list *bombs) {
    if (empty_list(bombs))
        return;
    int aux, size;
    t_item bomb;
    size_list(&size, bombs);
    initialize_current_first(bombs);
    for (aux=0; aux!=size; aux++) {
        check_current_item(&bomb, bombs);
        bomb.row++;
        if (bomb.row == ROW)
            bomb.state = DEAD;
        if (bomb.state == DEAD)
            remove_current_item(&bomb, bombs);
        else {
        	update_current_item(bomb, bombs);
            increment_current(bombs);
        }
    }
}

/* Move a nave mãe de maneira similar aos aliens. */
void move_mothership(t_item *mothership) {
    if (mothership->col != COL - 7)
        mothership->col++;
    else {
    	mothership->col = 2;
        mothership->state = DYING;
    }
}

/* Le o teclado e move ou atira o tanque de acordo. Se a tecla for q, termina o jogo. */
void move_tank(t_game *game) {
    game->player.dir=getch();
    switch (game->player.dir) {
        case KEY_LEFT:
            if (game->player.col > 1)
                game->player.col--;
            break;
        case KEY_RIGHT:
            if (game->player.col < COL-5)
                game->player.col++;
            break;
        case ' ':
            shoot(&game->player, &game->shots);
            break;
        case 'q':
            endgame(game);
    }
}
