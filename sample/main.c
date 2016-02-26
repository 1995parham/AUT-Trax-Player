/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 26-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/

//Tic-tac-toe playing AI. Exhaustive tree-search. WTFPL
//Matthew Steel 2009, www.www.repsilat.com

#include <stdio.h>


void playerMove(int board[9]) {
    int move = 0;
    do {
        printf("\nInput move ([0..8]): ");
        scanf("%d", &move);
        printf("\n");
    } while (move >= 9 || move < 0 && board[move] == 0);
    board[move] = -1;
}

int main() {
    int board[9] = {0,0,0,0,0,0,0,0,0};
    //computer squares are 1, player squares are -1.
    printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
    int player=0;
    scanf("%d",&player);
    printf("\n");
    unsigned turn;
    for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
        if((turn+player) % 2 == 0)
            computerMove(board);
        else {
            draw(board);
            playerMove(board);
        }
    }
    switch(win(board)) {
        case 0:
            printf("A draw. How droll.\n");
            break;
        case 1:
            draw(board);
            printf("You lose.\n");
            break;
        case -1:
            printf("You win. Inconceivable!\n");
            break;
    }
}
