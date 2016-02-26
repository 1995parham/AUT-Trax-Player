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
	X = 2,
	O = 1
};

enum state {
	WIN,
	LOSS,
	DRAW,
	NOTHING
};

struct game {
	const struct board *b;
	enum player p;
	enum player turn;
	enum state s;
};

struct game *game_new(enum player p, int row, int col);

enum state game_state(struct game *g);

void game_move(struct game *g, int row, int col);

void game_move_back(struct game *g, int row, int col);

#endif
