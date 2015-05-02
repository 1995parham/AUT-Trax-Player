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
#include "traxcore.h"
#include "AI.h"

void play(int player_no)
{
	char move[256];
	if (player_no == 1) {
		while (whoToMove() != NOPLAYER) {
			getRandomMove(move);
			printf("%s\n", move);
			makeMove(move);
			scanf("%s", move);
			makeMove(move);
		}
	} else {
		while (whoToMove() != NOPLAYER) {
			scanf("%s", move);
			makeMove(move);
			getRandomMove(move);
			printf("%s\n", move);
		}
	}
}
