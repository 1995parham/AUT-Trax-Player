/*
 * In The Name Of God
 * ========================================
 * [] File Name : AI
 *
 * [] Creation Date : 02-05-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include <string.h>
#include "traxcore.h"


static int notBadMoves(char result[][256])
{

	int size;
	int movesIndex = 0;
	int resultIndex = 0;
	char moves[100][256];

	size = uniqueMoves(1, moves);
	if (size == 1); /* return moves */

	/* start predicting moves */

	for (movesIndex = 0; movesIndex < size; movesIndex++) {
		const char *move = moves[movesIndex];
		makeMove(move);

		int gameOverValue = isGameOver();
		switch (gameOverValue) {
			case WHITE:
			case BLACK:
				/* Winning move found */
				if (whoDidLastMove() == gameOverValue) {
					strcpy(result[resultIndex], move);
					resultIndex++;
					return resultIndex;
				}
		                /* Losing move found */
		                break;
			case DRAW:
			case NOPLAYER:
				strcpy(result[resultIndex], move);
		                resultIndex++;
		                break;
			default:
				/* This should never happen */
				break;
		}
	}
	/* There are only bad moves ... :-(((( */
	if (resultIndex == 0) {
		strcpy(result[resultIndex], moves[0]);
		resultIndex++;
	}
	return resultIndex;
}

void getRandomMove(char move[256])
{
	int losingMoves = 0;
	int size;
	char moves[100][256];

	size = notBadMoves(moves);
	strcpy(move, moves[0]);
	/* return random moves */
}