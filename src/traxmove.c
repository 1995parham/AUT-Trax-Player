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
	aMove[0] = (char) (i + '@');
	aMove[1] = (char) (j + '0');
	aMove[2] = direction;
	aMove[3] = 0;
}

void getTraxMoveDefinition(int *i, int *j, const char *aMove, char *direction)
{
	*i = 0;
	do {
		char ch = *aMove++;
		*i = *i * 26 + ch - '@';
	} while (isalpha(*aMove));

	*j = atoi(aMove);
	while (isdigit(*aMove))
		aMove++;

	*direction = *aMove;
}