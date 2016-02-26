/*
 * In The Name Of God
 * ========================================
 * [] File Name : game.h
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef GAME_H
#define GAME_H

#include "board.h"

enum player {
	X = 0,
	Y = 1
};

enum state {
	WIN,
	LOSS,
	DRAW
};

struct gmae {
	const struct board *b;
	enum player p;
	enum player turn;
	enum state s;
};

struct game *game_new(enum player p, int row, int col);

enum state game_state(const struct game *g);

void game_move(int row, int col, int move);

#endif
