#include <string.h>
#include "printing_routines.h"

/* Checa se o projétil atingiu alguma barreira, e onde. */
int barrier_collision(t_list *barriers, t_item *missile) {
    int row, col, aux;
    t_item barrier;
    initialize_current_first(barriers);

    /* Checa todo elemento na lista de barreiras. */
    for (aux=0; aux!=N_BAR; aux++) {
        check_current_item(&barrier, barriers);
        row = missile->row - barrier.row;
        col = missile->col - barrier.col;

        /* Barreiras com dimensao 3x7. */
        if (missile->col >= barrier.col && missile->col < barrier.col + 7 && barrier.body[row*7+col] != ' ') {
            barrier.body[row*7+col] = ' ';
            missile->state = DEAD;
            update_current_item(barrier, barriers);
            return 1;
        }
        increment_current(barriers);
    }
    return 0;
}

/* Checa se o tiro acertou algum alien. Se sim, explode e mata ele. */
int alien_collision(t_list *mob, t_item *shot) {
    int aux, row, col;
    t_item alien;
    initialize_current_first(mob);

    /* Checa todo elemento na lista de aliens. */
    for (aux=0; aux!=MOB; aux++) {
        check_current_item(&alien, mob);
        row = shot->row - alien.row;
        col = shot->col - alien.col;

        /* Aliens com dimensao 3x3. */
        if (row < 3 && row >= 0 && col < 3 && col >= 0 && alien.state != DEAD) {
            alien.state = DYING;
            strcpy(alien.body, EXPLOSION);
            print_alien(&alien);
            update_current_item(alien, mob);
            return 1;
        }
        increment_current(mob);
    }
    return 0;
}

/* Verifica se houve colisão com o jogador. */
int player_collision(tank *player, t_item *bomb) {
	int row, col;
	row = bomb->row - player->row;
	col = player->col - bomb->col;
	if (row <= 3 && row > 0 && col > -5 && col <= 0) {
		return 1;
	}
	return 0;
}

/* Checa se o alien colidiu com a barreira. */
int barrier_alien_collision(t_item *barrier, t_item *alien) {
	int row, col;
	row = barrier->row - alien->row;
	col = alien->col - barrier->col;
	if (alien->dir == RIGHT) {
		if (col > -3 && col <= 4) {
			barrier->body[(2-row)*7+col+2] = ' ';
			return 1;
		}
	}
	else {
		if (col >= 0 && col < 7) {
			barrier->body[(2-row)*7+col] = ' ';
			return 1;
		}
	}
	return 0;
}

int check_shots(t_list *shots, t_list *barriers, t_list *mob, int *enemies) {
    t_item shot;
    int aux, n_shots, hits;
    size_list(&n_shots, shots);
    hits = 0;

    /* Analisa se algum tiro atingiu alguma barreira. */
    initialize_current_first(shots);
    for (aux=0; aux!=n_shots; aux++) {
        check_current_item(&shot, shots);
        if (shot.row > (ROW - 8) && shot.row < (ROW - 4) && shot.state == ALIVE)
            if (barrier_collision(barriers, &shot)) {
                remove_current_item(&shot, shots);
                decrement_current(shots);
            }
        increment_current(shots);
    }

    /* Analisa se algum tiro atingiu algum alien, e retorna em hits o número de aliens destruidos. */
    initialize_current_first(shots);
    size_list(&n_shots, shots);
    for (aux=0; aux!=n_shots; aux++) {
        check_current_item(&shot, shots);
        if (shot.state == ALIVE)
            if (alien_collision(mob, &shot)) {
            	hits++;
                remove_current_item(&shot, shots);
                decrement_current(shots);
                (*enemies)--;
            }
        increment_current(shots);
    }
    return hits;
}

int check_bombs(t_list *bombs, t_list *barriers, tank *player) {
	t_item bomb;
    int aux, n_bombs;
    size_list(&n_bombs, bombs);

    /* Analisa se alguma bomba atingiu alguma barreira */
	initialize_current_first(bombs);
    for (aux=0; aux!=n_bombs; aux++) {
        check_current_item(&bomb, bombs);
        if (bomb.row > (ROW - 8) && bomb.row < (ROW - 4) && bomb.state == ALIVE)
            if (barrier_collision(barriers, &bomb)) {
                remove_current_item(&bomb, bombs);
                decrement_current(bombs);
            }
        increment_current(bombs);
    }

    /* Analisa se alguma bomba atingiu o tanque. */
    initialize_current_first(bombs);
    for (aux=0; aux!=n_bombs; aux++) {
    	check_current_item(&bomb, bombs);
    	if (bomb.row > (ROW - 3) && bomb.row < ROW) {
    		if (player_collision(player, &bomb))
    			return DEAD;
   	 	}
    	increment_current(bombs);
    }
    return ALIVE;
}

/* Checa colisao entre projeteis */
int check_missiles(t_list *bombs, t_list *shots) {
    /* Declara variaveis de apoio e inicializa tamanhos. */
    t_item bomb, shot;
    int size_b, size_s, aux, aux2;
    size_list(&size_b, bombs);
    size_list(&size_s, shots);
    if (size_b == 0 || size_s == 0)
        return 0;

    /* Percorre as duas listas checando as colisões. */
    initialize_current_first(shots);
    for (aux=0; aux!=size_s; aux++) {
        check_current_item(&shot, shots);
        initialize_current_first(bombs);

        /* Percorre as bombas. */
        for (aux2=0; aux2!=size_b; aux2++) {
            check_current_item(&bomb, bombs);
            if (shot.col == bomb.col && shot.row - bomb.row <= 1) {
                remove_current_item(&bomb, bombs);
                remove_current_item(&shot, shots);
                break;
            }
            increment_current(bombs);
        }
        increment_current(shots);
    }
    return 1;
}

void check_barriers(t_list *barriers, t_list *mob) {
	t_item bar, alien;
	int aux, aux2;

	initialize_current_first(barriers);
	for (aux=0; aux!=N_BAR; aux++) {
		check_current_item(&bar, barriers);
		initialize_current_first(mob);
		for (aux2=0; aux2!=MOB; aux2++) {
			check_current_item(&alien, mob);
			if (alien.row > (ROW - 12) && alien.row < (ROW - 2) && alien.state == ALIVE) {
				barrier_alien_collision(&bar, &alien);
				update_current_item(bar, barriers);
			}
			increment_current(mob);
		}
		increment_current(barriers);
	}
}

/* Checa se a nave mãe foi morta. Como apenas um tiro pode estar em cada coluna por vez,
   e o último tiro a sair está na primeira posição e é o mais alto, ele é o único que pode
   atingir a nave mãe. */
int mothership_kill(t_list *shots, t_item *mothership) {
    t_item shot;
    int col;
    initialize_current_first(shots);
    check_current_item(&shot, shots);
    col = shot.col - mothership->col;
    if (shot.row <= 4 && col < 7 && col >= 0 && mothership->state == ALIVE) {
        mothership->state = DYING;
        remove_current_item(&shot, shots);
        return MOTHERSHIP_SCORE;
    }
    return 0;
}

/* Checa as posições de todos os tiros e bombas, e caso eles atingiram alguma entidade,
   atualiza a entidade de acordo. */
void check_collisions(t_game *game, int max_row) {

    /* Checa tiros do jogador. Se atingiu algum alien, atualiza os pontos */
    game->score = game->score + ALIEN_SCORE*check_shots(&game->shots, &game->barriers, &game->mob, &game->alive_enemies);

    /* Checa bombas. Se atingiu o jogador, atualiza o estado do jogo de acordo. */
   	game->state = check_bombs(&game->bombs, &game->barriers, &game->player);

    /* Checa se os tiros colidiram com as bombas. */
    check_missiles(&game->bombs, &game->shots);

    if (max_row >= (ROW - 9))
    	check_barriers(&game->barriers, &game->mob);

    /* Checa se a nave mãe foi morta. */
    if (game->mothership.state == ALIVE)
    	game->score = game->score + mothership_kill(&game->shots, &game->mothership);

}
