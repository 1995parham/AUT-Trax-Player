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
#include "traxmove.h"

void getTraxString(int i, int j, char *aMove, char direction)
{
	aMove[0] = (char) (i + '@');
	aMove[1] = (char) (j + '0');
	aMove[2] = direction;
	aMove[3] = 0;
}
