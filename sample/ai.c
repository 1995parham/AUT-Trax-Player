/*
 * In The Name Of God
 * ========================================
 * [] File Name : ai.c
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
#include "player.h"

int minimax(struct game *g) {
	/* How is the position like for player (their turn) on board? */
	enum player p = g->turn;
	enum state s = game_state(g);
	
	if (s == WIN)
		return 1;
	else if (s == LOSS)
		return -1;

    	struct move m = {-1, -1};
    	int score = 0;//Losing moves are preferred to no move
    	int i, j;
    	/* For all moves */
    	for (i = 0; i < g->b->row; i++) {
		for (j = 0; j < g->b->col; j++) {
			/* If legal move */
			if (board_get_cell(g->b, i, j) == 0) {
				/* Try the move */
				game_move(g, i, j);

				int current = minimax(g);
				
				/* Pick the one that's worst for the opponent */
				if(score == 0 || current == -1) {
					score = -current;
					m.row = i;
					m.col = j;
				}				

				/* Reset board after try */
				game_move_back(g, i, j);
			}
		}
	}

	if(m.row == -1 && m.col == -1)
		return 0;
	return score;
}

void computerMove(int board[9]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            //int tempScore = -minimax(board, -1);
	    int tempScore = 0;
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
}

