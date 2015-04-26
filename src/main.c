/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 26-04-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include "string.h"
#include "game.h"

int main(int argc, char *argv[])
{
	char input[4];
	int player_no;

	/* input = serial input */

	if (!strcmp(input, "-W"))
		player_no = 1;
	else
		player_no = 2;
	play(player_no);
}
