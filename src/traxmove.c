/*
 * In The Name Of God
 * ========================================
 * [] File Name : traxmove.c
 *
 * [] Creation Date : 01-05-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include <ctype.h>
#include <stdlib.h>
#include "traxmove.h"

void getTraxMoveString(int i, int j, char *aMove, char direction)
{
	int index = 0;

	if (i == 0) {
		aMove[index] = '@';
		index++;
	} else {
		do {
			i--;
			aMove[index] = (char) ((i % 26) + 'A');
			index++;
			i /= 26;
		} while (i != 0);
	}

	do {
		aMove[index] = (char) ((j % 10) + '0');
		index++;
		j /= 10;
	} while (j != 0);

	aMove[index] = direction;
	index++;
	aMove[index] = 0;
}

void getTraxMoveDefinition(int *i, int *j, const char *aMove, char *direction)
{
	*i = *aMove++ - '@';
	while (isalpha(*aMove)) {
		char ch = *aMove++;
		*i = *i * 26 + ch - 'A';
	};

	*j = atoi(aMove);
	while (isdigit(*aMove))
		aMove++;

	*direction = *aMove;
}