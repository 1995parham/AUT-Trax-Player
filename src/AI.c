/*
 * In The Name Of God
 * ========================================
 * [] File Name : AI.c
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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "traxcore.h"


static int notBadMoves(char result[][256])
{

	int size;
	int movesIndex = 0;
	int resultIndex = 0;
	char moves[100][256];

	size = uniqueMoves(moves);
	if (size == 1) {
		strcpy(result[resultIndex], moves[0]);
		resultIndex++;
		return resultIndex;
	}

	/* print all moves for debugging purpose only */
#ifdef DEBUG
	for (movesIndex = 0; movesIndex < size; movesIndex++)
		printf("%s\n", moves[movesIndex]);
#endif


	/* start predicting moves */
	for (movesIndex = 0; movesIndex < size; movesIndex++) {
		saveState();
		const char *move = moves[movesIndex];

		if (makeMove(move) < 0)
			continue;

		int gameOverValue = isGameOver();
		switch (gameOverValue) {
			case WHITE:
			case BLACK:
				/* Winning move found */
				if (whoDidLastMove() == gameOverValue) {
#ifdef DEBUG
					printf("Wining move found %s\n", move);
#endif
					resultIndex = 0;
					strcpy(result[resultIndex], move);
					resultIndex++;
					restoreState();
					return resultIndex;
				}
		                /* Loosing move found */
		                break;
			case DRAW:
			case NOPLAYER:
				/* Unknown move found */
				strcpy(result[resultIndex], move);
		                resultIndex++;
		                restoreState();
			default:
				/* This should never happen */
				break;
		}
	}
	/*
	 * There are only bad moves ... :-((((
	 * Lets go home....
	*/
	if (resultIndex == 0) {
		strcpy(result[resultIndex], moves[0]);
		resultIndex++;
	}
	return resultIndex;
}

void getRandomMove(char move[256])
{
	int goodMoves = 0;
	char moves[100][256];

	srand((unsigned int) time(NULL));
	goodMoves = notBadMoves(moves);
	if (goodMoves > 1)
		strcpy(move, moves[rand() % goodMoves]);
	else
		strcpy(move, moves[goodMoves - 1]);
	/* return random moves */
}