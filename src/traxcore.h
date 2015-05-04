/*
 * In The Name Of God
 * ========================================
 * [] File Name : traxboard.h
 *
 * [] Creation Date : 26-04-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include "limits.h"

#ifndef TRAXBOARD_H
#define TRAXBOARD_H

#define EMPTY 0
#define INVALID 7
#define NS 1
#define SN 1
#define WE 2
#define EW 2
#define NW 3
#define WN 3
#define NE 4
#define EN 4
#define WS 5
#define SW 5
#define SE 6
#define ES 6

#define WHITE 0
#define BLACK 1
#define DRAW 2
#define NOPLAYER 3
#define NORESULT 3


struct trax_game {
	int boardEmpty;
	int wtm;
	int board[2 * BOARD_SIZE + 1][2 * BOARD_SIZE + 1];
	int gameOver;
	int numOfTiles;
	int firstRow, lastRow, firstCol, lastCol;

};

int makeMove(const char *move);

int whoToMove(void);

int whoDidLastMove(void);

void saveState(void);

void restoreState(void);

int isGameOver(void);

/*
 * use array at least can contains
 * 100 moves with 256 character
*/
int uniqueMoves(int remove_mirror_moves, char moves[][256]);

#endif