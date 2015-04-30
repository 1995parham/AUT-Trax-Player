/*
 * In The Name Of God
 * ========================================
 * [] File Name : game.c
 *
 * [] Creation Date : 26-04-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include <stdio.h>
#include "game.h"
#include "traxboard.h"

void play(int player_no)
{
	char move[256];
	if (player_no == 1) {
		while (whoToMove() != NOPLAYER) {
			/* send move */
			printf("Hello");
			scanf("%s", move);
			makeMove(move);
		}
	} else {
		while (whoToMove() != NOPLAYER) {
			scanf("%s", move);
			makeMove(move);
			printf("Hello");
			/* send move */
		}
	}
}
