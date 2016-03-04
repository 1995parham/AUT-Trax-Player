/*
 * In The Name Of God
 * ========================================
 * [] File Name : player.h
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef PLAYER_H
#define PLAYER_H

struct move {
	int row;
	int col;
};

struct player_c {
	void *context;
	struct move (*do_move)(struct player_c *p, struct move enemy_move);
}

#endif
