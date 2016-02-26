/*
 * In The Name Of God
 * ========================================
 * [] File Name : board.h
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

struct board {
	int *b;
	int row, col;
};

const struct board *board_new(int row, int col);

void board_print(const struct board *b, FILE *fp);

int board_get_cell(const struct board *b, int row, int col);

void board_set_cell(const struct board *b, int row, int col, int v);

void board_delete(const struct board *b);

#endif
