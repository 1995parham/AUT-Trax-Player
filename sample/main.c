/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include "player.h"
#include "game.h"
#include "ai.h"
#include "ui.h"

#include <stdio.h>

int main(int argc, char *argv)
{
	int choose;
	struct player_c *first, *second;
	struct game *g;

	printf("Please choose first player [AI(1) or UI(2)]: ");
	scanf("%d", &choose);
	
	if (choose == 1)
		first = ai_player_new(X, 3, 3);
	else
		first = ui_player_new(X, 3, 3);

	printf("Please choose second player [AI(1) or UI(2)]: ");
	scanf("%d", &choose);

	if (choose == 1)
		second = ai_player_new(X, 3, 3);
	else
		second = ui_player_new(X, 3, 3);

	g = game_new(X, 3, 3);
	struct move m = {-1, -1};

	while (game_state(g) == NOTHING) {
		if (g->turn == X)
			m = first->do_move(first, m);
		else
			m = second->do_move(second, m);
		game_move(g, m.row, m.col);
	}

	if (game_state(g) == WIN)
		printf("Player X was won\n");
	else if (game_state(g) == LOSS)
		printf("Player O was won\n");
	else
		printf("Draw\n");
}
