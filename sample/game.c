/*
 * In The Name Of God
 * ========================================
 * [] File Name : game.c
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include "game.h"
#include "board.h"

#include <stdlib.h>

struct game *game_new(enum player p, int row, int col)
{
	struct game *new;

	new = malloc(sizeof(struct game));
	
	new->b = board_new(row, col);
	new->p = p;
	new->turn = X;
	new->s = NOTHING;
}

enum state game_state(struct game *g)
{
	int i, j, counter;
	/* check rows for X */
	for (i = 0; i < g->b->row; i++) {
		counter = 0;
		for (j = 0; j < g->b->col && board_get_cell(g->b, i, j) == 2; j++) {
			counter++;
		}
		if (counter == g->b->col) {
			if (g->p == X)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}
	/* check columns for X */
	for (j = 0; j < g->b->col; j++) {
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, i, j) == 2; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == X)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}
	/* check diameters for X */
	if (g->b->row == g->b->col) {
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, i, i) == 2; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == X)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, g->b->row - 1 - i, i) == 2; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == X)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}
	/* check rows for X */
	for (i = 0; i < g->b->row; i++) {
		counter = 0;
		for (j = 0; j < g->b->col && board_get_cell(g->b, i, j) == 1; j++) {
			counter++;
		}
		if (counter == g->b->col) {
			if (g->p == O)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}
	/* check columns for X */
	for (j = 0; j < g->b->col; j++) {
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, i, j) == 1; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == O)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}
	/* check diameter for X */
	if (g->b->row == g->b->col) {
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, i, i) == 1; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == O)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
		counter = 0;
		for (i = 0; i < g->b->row && board_get_cell(g->b, g->b->row - 1 - i, i) == 1; i++) {
			counter++;
		}
		if (counter == g->b->row) {
			if (g->p == O)
				g->s = WIN;
			else
				g->s = LOSS;
			return g->s;
		}
	}

	for (i = 0; i < g->b->row; i++) {
		for (j = 0; j < g->b->col; j++) {
			if (board_get_cell(g->b, i, j) == 0) {
				g->s = NOTHING;
				return g->s;
			}
		}
	}
	g->s = DRAW;
	return g->s;
}

void game_move(struct game *g, int row, int col)
{
	if (g->turn == X) {
		board_set_cell(g->b, row, col, 2);
		g->turn = O;
	} else {
		board_set_cell(g->b, row, col, 1);
		g->turn = X;
	}

	game_state(g);
}

void game_move_back(struct game *g, int row, int col)
{
	if (g->turn == X) {
		board_set_cell(g->b, row, col, 0);
		g->turn = O;
	} else {
		board_set_cell(g->b, row, col, 0);
		g->turn = X;
	}

	game_state(g);
}
