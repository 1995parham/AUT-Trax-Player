/*
 * In The Name Of God
 * ========================================
 * [] File Name : ui.c
 *
 * [] Creation Date : 05-03-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/

#include "board.h"
#include "game.h"
#include "ui.h"
#include "player.h"

#include <stdlib.h>
#include <stdio.h>

static struct move move(struct player_c *p, struct move enemy_move) {
	struct game *g = (struct game *) p->context;
	
	/* check for enemy_move */
	if (enemy_move.row != -1 && enemy_move.col != -1)
		game_move(g, enemy_move.row, enemy_move.col);

	struct move m = {-1, -1};

	board_print(g->b, stdout);

	do {
		printf("Please enter your move row and col [%c]: ", (g->p == X) ? 'X' : 'O');
		scanf("%d %d", &m.row, &m.col);
	} while (m.row >= g->b->row || m.col >= g->b->col || board_get_cell(g->b, m.row, m.col) != 0);
	
	/* do our move */
	game_move(g, m.row, m.col);

	return m;
}


struct player_c *ui_player_new(enum player p, int row, int col)
{
	struct player_c *new = malloc(sizeof(struct player_c));
	new->do_move = move;
	new->context = game_new(p, row, col);

	return new;
}
