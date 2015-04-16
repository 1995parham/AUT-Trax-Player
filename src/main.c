/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 16-04-2015
 *
 * [] Last Modified : Thu 16 Apr 2015 04:31:10 PM IRDT
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
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
