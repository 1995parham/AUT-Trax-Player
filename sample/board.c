/*
 * In The Name Of God
 * ========================================
 * [] File Name : board.c
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

const struct board *board_new(int row, int col)
{
	struct board *new;
	int i, j;
	
	new = malloc(sizeof(struct board));
	new->row = row;
	new->col = col;
	new->b = malloc(row * col * sizeof(int));

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			new->b[i * col + j] = 0;
		}
	}

	return new;
}

int board_get_cell(const struct board *b, int row, int col)
{
	if (row < b->row && 0 <= row)
		if (col < b->col && 0 <= col)
			return b->b[row * b->col + col];
	return -1;
}

void board_set_cell(const struct board *b, int row, int col, int v)
{
	if (row < b->row && 0 <= row)
		if (col < b->col && 0 <= col)
			b->b[row * b->col + col] = v;
	return;
}

void board_delete(const struct board *b)
{
	free(b->b);
	free((void *) b);
}


static char board_print_cell(const struct board *b, int row, int col)
{
	switch(board_get_cell(b, row, col)) {
        case 0:
		return ' ';
        case 1:
		return 'O';
	case 2:
		return 'X';
	}
	return ' ';
}

void board_print(const struct board *b, FILE *fp)
{
	int i, j;

	fprintf(fp, "+");
	for (j = 0; j < b->col; j++)
		fprintf(fp, "---+");
	fprintf(fp, "\n");
	for (i = 0; i < b->row; i++) {
		fprintf(fp, "|");
		for (j = 0; j < b->col; j++)
			fprintf(fp, " %c |", board_print_cell(b, i, j));
		fprintf(fp, "\n");
		fprintf(fp, "+");
		for (j = 0; j < b->col; j++)
			fprintf(fp, "---+");
		fprintf(fp, "\n");
	}
}


