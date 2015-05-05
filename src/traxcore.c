/*
 * In The Name Of God
 * ========================================
 * [] File Name : traxboard.c
 *
 * [] Creation Date : 26-04-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#include <string.h>

#include "traxcore.h"
#include "traxmove.h"

#define false 0
#define true 1

static struct trax_game current = {
	.boardEmpty = 1,
	.wtm = WHITE,
	.gameOver = NOPLAYER,
	.boardEmpty = 1,
	.firstCol = 0,
	.firstRow = 0,
	.numOfTiles = 0,
	.lastCol = 0,
	.lastRow = 0
};

static struct trax_game save;

static int getNumOfTiles(void)
{
	return current.numOfTiles;
}

void saveState(void)
{
	save = current;
}

void restoreState(void)
{
	current = save;
}

int getRowSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (current.lastRow -
	                                          current.firstRow));
}

int getColSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (current.lastCol -
	                                          current.firstCol));
}

static char getAt(int row, int col)
{
	if ((row < 1) || (row > BOARD_SIZE))
		return EMPTY;
	if ((col < 1) || (col > BOARD_SIZE))
		return EMPTY;
	return current.board[current.firstRow + row - 1][current.firstCol +
	                                                 col - 1];
}

static int isBlank(int row, int col)
{
	return (getAt(row, col) == EMPTY);
}

static void putAt(int row, int col, char piece)
{
	if (piece == EMPTY) {
		if (current.board[current.firstRow + row - 1][current.firstCol +
		                                              col - 1] != EMPTY)
			current.numOfTiles--;
		current.board[current.firstRow + row - 1][current.firstCol +
		                                          col - 1] = piece;
		return;
	} else {
		if (current.boardEmpty) {
			current.boardEmpty = false;
			current.firstRow = BOARD_SIZE - 1;
			current.firstCol = BOARD_SIZE - 1;
			current.lastRow = BOARD_SIZE - 1;
			current.lastCol = BOARD_SIZE - 1;
			current.numOfTiles = 1;
			current.board[current.firstRow][current.firstCol] = piece;
			return;
		}
		if (row == 0) {
			current.firstRow--;
			row++;
		}
		if (col == 0) {
			current.firstCol--;
			col++;
		}
		if (row > getRowSize()) {
			current.lastRow += row - getRowSize();
		}
		if (col > getColSize()) {
			current.lastCol += col - getColSize();
		}
		current.numOfTiles++;
	}
	current.board[current.firstRow + row - 1][current.firstCol + col -
	                                          1] = piece;
}

static void switchPlayer()
{
	switch (current.wtm) {
		case WHITE:
			current.wtm = BLACK;
	                break;
		case BLACK:
			current.wtm = WHITE;
	                break;
		default:
			break;
	                /* This should never happen */
	}
}

static int canMoveDown(void)
{
	return (getRowSize() < BOARD_SIZE);
}

static int canMoveRight()
{
	return (getColSize() < BOARD_SIZE);
}

static int checkLine(int row, int col, char direction, char type)
{
	/*
	 * type can be
	 * h -> horizontal
	 * v -> vertical
	 * l -> loop
	*/

	int start_row = row;
	int start_col = col;
	int ix = 0;
	const char *newDirection;

	newDirection = " uurllrddlrrlllduudrruddu";

	while (true) {
		/*
		 * no line starts with a empty space
		 * or we are out of range
		*/
		if (isBlank(row, col))
			return false;
		switch (direction) {
			case 'u':
				/* newDirection's first line */
				ix = 0;
		                break;
			case 'd':
				/* newDirection's second line */
				ix = 6;
		                break;
			case 'l':
				/* newDirection's third line */
				ix = 12;
		                break;
			case 'r':
				/* newDirection's fourth line */
				ix = 18;
		                break;
			default:
				break;
		}
		ix += getAt(row, col);
		direction = newDirection[ix];
		switch (direction) {
			case 'u':
				row--;
		                break;
			case 'd':
				row++;
		                break;
			case 'l':
				col--;
		                break;
			case 'r':
				col++;
		                break;
			default:
				break;
		}
		if ((row == start_row) && (col == start_col)) {
			/* loop win */
			return type == 'l';
		}
	}
}

int isGameOver(void)
{
	int WhiteWins = false, BlackWins = false;

	if (current.gameOver != NOPLAYER)
		return current.gameOver;
	if (current.numOfTiles < 4) {
		current.gameOver = NOPLAYER;
		return current.gameOver;
	}

	/* check loop wins */
	int i, j;
	for (i = 1; i < BOARD_SIZE; i++) {
		for (j = 1; j < BOARD_SIZE; j++) {
			switch (getAt(i, j)) {
				case NW:
					if (checkLine(i, j, 'u', 'l'))
						BlackWins = true;
			                break;
				case SE:
					if (checkLine(i, j, 'u', 'l'))
						WhiteWins = true;
			                break;
				case EMPTY:
				case NS:
				case WE:
				case NE:
				case WS:
					break;
				default:
					break;
			                /* This should never happen */
			}
		}
	}

	if (WhiteWins && BlackWins) {
		current.gameOver = whoDidLastMove();
		return current.gameOver;
	}
	if (WhiteWins) {
		current.gameOver = WHITE;
		return current.gameOver;
	}
	if (BlackWins) {
		current.gameOver = BLACK;
		return current.gameOver;
	}
	return NOPLAYER;
}

static int forcedMove(int brow, int bcol)
{
	if (!isBlank(brow, bcol)) return true;
	if ((brow < 1) || (brow > BOARD_SIZE) || (bcol < 1) ||
	    (bcol > BOARD_SIZE))
		return true;

	int up = getAt(brow - 1, bcol);
	int down = getAt(brow + 1, bcol);
	int left = getAt(brow, bcol - 1);
	int right = getAt(brow, bcol + 1);

	int neighbors = 0;

	if (!up) neighbors++;
	if (!down) neighbors++;
	if (!left) neighbors++;
	if (!right) neighbors++;

	if (neighbors < 2)
		/* Less than two pieces bordering */
		return true;

	int white_up = 0, black_up = 0, white_down = 0, black_down = 0, white_left = 0, black_left = 0, white_right = 0, black_right = 0;

	if (up == SN || up == SW || up == SE) white_up = 1;
	if (up == WE || up == NW || up == NE) black_up = 1;
	if (down == NS || down == NW || down == NE) white_down = 1;
	if (down == WE || down == SW || down == SE) black_down = 1;
	if (left == EW || left == EN || left == ES) white_left = 1;
	if (left == NS || left == NW || left == SW) black_left = 1;
	if (right == WE || right == WN || right == WS) white_right = 1;
	if (right == NS || right == NE || right == SE) black_right = 1;

	int white = white_up + white_down + white_left + white_right;
	int black = black_up + black_down + black_left + black_right;

	if ((white > 2) ||
	    (black > 2)) { return false; } // Illegal filled cave
	if ((white < 2) && (black < 2)) { return true; } // Done

	char piece = EMPTY;
	if (white == 2) {
		switch (white_up + 2 * white_down + 4 * white_left +
		        8 * white_right) {
			case 3:
				piece = NS;
		                break;
			case 12:
				piece = WE;
		                break;
			case 5:
				piece = NW;
		                break;
			case 9:
				piece = NE;
		                break;
			case 6:
				piece = WS;
		                break;
			case 10:
				piece = SE;
		                break;
			default:
				break;
		}
	} else {
		switch (black_up + 2 * black_down + 4 * black_left +
		        8 * black_right) {
			case 12:
				piece = NS;
		                break;
			case 3:
				piece = WE;
		                break;
			case 10:
				piece = NW;
		                break;
			case 6:
				piece = NE;
		                break;
			case 9:
				piece = WS;
		                break;
			case 5:
				piece = SE;
		                break;
			default:
				break;
		}
	}
	putAt(brow, bcol, piece);
	if (!forcedMove(brow - 1, bcol)) { return false; }
	if (!forcedMove(brow + 1, bcol)) { return false; }
	if (!forcedMove(brow, bcol - 1)) { return false; }
	return forcedMove(brow, bcol + 1);
}

int makeMove(const char *move)
{
	int col, row, neighbor;
	char direction;
	int ohs_up = 0, ohs_down = 0, ohs_right = 0, ohs_left = 0,
		eks_up = 0, eks_down = 0, eks_right = 0, eks_left = 0;

	if (current.gameOver != NOPLAYER); /* Game is over */

	/* move = move.toUpperCase(); */

	if (current.boardEmpty) {
		if (!strcmp(move, "@0/")) {
			putAt(1, 1, NW);
			switchPlayer();
			return 0;
		}
		if (!strcmp(move, "@0+")) {
			putAt(1, 1, NS);
			switchPlayer();
			return 0;
		}
	}

	/* parse move string */
	getTraxMoveDefinition(&row, &col, move, &direction);

	if (col == 0 && row == 0)
		return -1; /* No neighbours */
	if ((row == 0) && (!canMoveDown()))
		return -1; /* illegal row */
	if ((col == 0) && (!canMoveRight()))
		return -1; /* illegal column */
	if (!isBlank(row, col))
		return -1; /* occupy */

	int up = getAt(row - 1, col), down = getAt(row + 1,
		col), left = getAt(
		row, col - 1), right = getAt(row,
		col + 1);

	if (up == SN || up == SE || up == SW) ohs_up = 1;
	if (up == EW || up == NW || up == NE) eks_up = 1;
	if (down == NS || down == NE || down == NW) ohs_down = 1;
	if (down == EW || down == SW || down == SE) eks_down = 1;
	if (left == EN || left == ES || left == EW) ohs_left = 1;
	if (left == WS || left == WN || left == NS) eks_left = 1;
	if (right == WN || right == WE || right == WS) ohs_right = 1;
	if (right == ES || right == NS || right == EN) eks_right = 1;
	neighbor = ohs_up + (2 * ohs_down) + (4 * ohs_left) +
	           (8 * ohs_right)
	           + (16 * eks_up) + (32 * eks_down) + (64 * eks_left) +
	           (128 * eks_right);

	switch (neighbor) {
		case 0:
			/* no neighbor error */
			return -1;
		case 1:
			switch (direction) {
				case '/':
					putAt(row, col, NW);
			                break;
				case '\\':
					putAt(row, col, NE);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 2:
			switch (direction) {
				case '/':
					putAt(row, col, SE);
			                break;
				case '\\':
					putAt(row, col, SW);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 4:
			switch (direction) {
				case '/':
					putAt(row, col, WN);
			                break;
				case '\\':
					putAt(row, col, WS);
			                break;
				case '+':
					putAt(row, col, WE);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 8:
			switch (direction) {
				case '/':
					putAt(row, col, ES);
			                break;
				case '\\':
					putAt(row, col, EN);
			                break;
				case '+':
					putAt(row, col, EW);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 16:
			switch (direction) {
				case '/':
					putAt(row, col, SE);
			                break;
				case '\\':
					putAt(row, col, SW);
			                break;
				case '+':
					putAt(row, col, WE);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 18:
			switch (direction) {
				case '/':
					putAt(row, col, SE);
			                break;
				case '\\':
					putAt(row, col, SW);
			                break;
				case '+':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 20:
			switch (direction) {
				case '/':
					return -1;
				case '\\':
					putAt(row, col, WS);
			                break;
				case '+':
					putAt(row, col, WE);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 24:
			switch (direction) {
				case '/':
					putAt(row, col, SE);
			                break;
				case '\\':
					return -1;
				case '+':
					putAt(row, col, WE);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 32:
			switch (direction) {
				case '/':
					putAt(row, col, NW);
			                break;
				case '\\':
					putAt(row, col, NE);
			                break;
				case '+':
					putAt(row, col, WE);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 33:
			switch (direction) {
				case '/':
					putAt(row, col, NW);
			                break;
				case '\\':
					putAt(row, col, NE);
			                break;
				case '+':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 36:
			switch (direction) {
				case '/':
					putAt(row, col, NW);
			                break;
				case '+':
					putAt(row, col, WE);
			                break;
				case '\\':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 40:
			switch (direction) {
				case '\\':
					putAt(row, col, EN);
			                break;
				case '+':
					putAt(row, col, EW);
			                break;
				case '/':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 64:
			switch (direction) {
				case '/':
					putAt(row, col, ES);
			                break;
				case '\\':
					putAt(row, col, EN);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 65:
			switch (direction) {
				case '\\':
					putAt(row, col, NE);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				case '/':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 66:
			switch (direction) {
				case '/':
					putAt(row, col, SE);
			                break;
				case '+':
					putAt(row, col, SN);
			                break;
				case '\\':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 72:
			switch (direction) {
				case '/':
					putAt(row, col, ES);
			                break;
				case '\\':
					putAt(row, col, EN);
			                break;
				case '+':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 128:
			switch (direction) {
				case '/':
					putAt(row, col, WN);
			                break;
				case '\\':
					putAt(row, col, WS);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 129:
			switch (direction) {
				case '/':
					putAt(row, col, NW);
			                break;
				case '+':
					putAt(row, col, NS);
			                break;
				case '\\':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 130:
			switch (direction) {
				case '\\':
					putAt(row, col, SW);
			                break;
				case '+':
					putAt(row, col, SN);
			                break;
				case '/':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		case 132:
			switch (direction) {
				case '/':
					putAt(row, col, WN);
			                break;
				case '\\':
					putAt(row, col, WS);
			                break;
				case '+':
					return -1;
				default:
					/* This should never happen */
					break;
			}
	                break;
		default:
			/* This should never happen */
			break;
	}
	if (row == 0) row++;
	if (col == 0) col++;
	if (!forcedMove(row - 1, col)) {
		return -1;
	}
	if (!forcedMove(row + 1, col)) {
		return -1;
	}
	if (!forcedMove(row, col - 1)) {
		return -1;
	}
	if (!forcedMove(row, col + 1)) {
		return -1;
	}
	/* note that switchPlayer() _must_ come before isGameOver() */
	switchPlayer();
	/* updates the gameOver attribute */
	isGameOver();
	return 0;

}

int whoToMove(void)
{
	return current.wtm;
}

int whoDidLastMove(void)
{
	if (current.boardEmpty)
		return NOPLAYER;
	switch (current.wtm) {
		case WHITE:
			return BLACK;
		case BLACK:
			return WHITE;
		default:
			break;
	                /* This should never happen */
	}
	return NOPLAYER;
}


static int isRotateMirror()
{
	char piece;
	int i, j, i2, j2;

	i2 = getRowSize();
	for (i = 1; i <= ((getRowSize() + 1) / 2); i++) {
		j2 = getColSize();
		for (j = 1; j <= getColSize(); j++) {
			piece = getAt(i, j);
			switch (getAt(i2, j2)) {
				case NW:
					if (piece != SE)
						return false;
			                break;
				case NE:
					if (piece != SW)
						return false;
			                break;
				case SW:
					if (piece != NE)
						return false;
			                break;
				case SE:
					if (piece != NW)
						return false;
			                break;
				case NS:
					if (piece != NS)
						return false;
			                break;
				case WE:
					if (piece != WE)
						return false;
			                break;
				case EMPTY:
					if (piece != EMPTY)
						return false;
			                break;
				default:
					break;
			}
			j2--;
		}
		i2--;
	}
	return true;
}


int uniqueMoves(char moves[][256])
{
	/*
	 * complex throw away a lot of equal moves
	 * and symmetries (hopefully)
	*/

	int movesIndex = 0;

	int i, j, k;
	char dl, dr, ur, rr, ul;
	/* which neighbors - default all values 0 */
	int neighbors[BOARD_SIZE + 2][BOARD_SIZE + 2];
	int directionList[BOARD_SIZE + 2][BOARD_SIZE + 2][3];
	/*
	 * which directions for move
	 * 0 -> /
	 * 1 -> \
	 * 2 -> +
	 * true means already used
	 * default all values false
	*/
	int ohs_up, ohs_down, ohs_right, ohs_left, eks_up, eks_down, eks_right, eks_left;
	int up, down, left, right;
	int iBegin, jBegin, iEnd, jEnd;

	int rsym;

	if (current.gameOver != NOPLAYER)
		return 0;

	/* empty board only two moves */
	if (current.boardEmpty) {
		strcpy(moves[movesIndex], "@0/");
		movesIndex++;
		strcpy(moves[movesIndex], "@0+");
		movesIndex++;
		return movesIndex;
	}
	if (getRowSize() * getColSize() == 1) {
		switch (getAt(1, 1)) {
			case NW:
				strcpy(moves[movesIndex], "@1+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "@1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "@1\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "B1+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "B1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "B1\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A2/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A2\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A2+");
		                movesIndex++;
		                break;
			case NS:
				strcpy(moves[movesIndex], "@1+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "@1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "@1\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "B1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "B1\\");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A2/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A2\\");
		                movesIndex++;
		                break;
			default:
				/* This should never happen */
				break;
		}
		return movesIndex;
	}

	for (i = 0; i < BOARD_SIZE + 2; i++)
		for (j = 0; j < BOARD_SIZE + 2; j++)
			for (k = 0; k < 3; k++)
				directionList[i][j][k] = false;

	rsym = isRotateMirror();
	iBegin = (canMoveDown()) ? 0 : 1;
	jBegin = (canMoveRight()) ? 0 : 1;
	iEnd = (getRowSize() < BOARD_SIZE) ? getRowSize() : BOARD_SIZE;
	jEnd = (getColSize() < BOARD_SIZE) ? getColSize() : BOARD_SIZE;

	for (i = iBegin; i <= iEnd; i++) {
		for (j = jBegin; j <= jEnd; j++) {
			if (!(isBlank(i, j))) {
				neighbors[i][j] = 0;
			} else {
				ohs_up = 0;
				ohs_down = 0;
				ohs_right = 0;
				ohs_left = 0;
				eks_up = 0;
				eks_down = 0;
				eks_right = 0;
				eks_left = 0;
				up = getAt(i - 1, j);
				down = getAt(i + 1, j);
				left = getAt(i, j - 1);
				right = getAt(i, j + 1);

				if (up == SN || up == SW || up == SE) {
					ohs_up = 1;
				} else if (up != EMPTY) {
					eks_up = 1;
				}

				if (down == NS || down == NW ||
				    down == NE) {
					ohs_down = 1;
				} else if (down != EMPTY) {
					eks_down = 1;
				}

				if (left == EN || left == ES ||
				    left == WE)
					ohs_left = 1;
				else if (left != EMPTY)
					eks_left = 1;

				if (right == WE || right == WS ||
				    right == WN)
					ohs_right = 1;
				else if (right != EMPTY)
					eks_right = 1;

				neighbors[i][j] =
					ohs_up + 2 * ohs_down +
					4 * ohs_left
					+ 8 * ohs_right + 16 * eks_up +
					32 * eks_down + 64 * eks_left +
					128 * eks_right;
			}
		}
	}

	for (i = iBegin; i <= iEnd; i++) {
		for (j = jBegin; j <= jEnd; j++) {
			if (neighbors[i][j] != 0) {
				dl = getAt(i + 1, j - 1);
				dr = getAt(i + 1, j + 1);
				ur = getAt(i - 1, j + 1);
				ul = getAt(i - 1, j - 1);
				rr = getAt(i, j + 2);
				switch (neighbors[i][j]) {
					case 1:
						if (dr == NS || dr == NW ||
						    dr == NE)
							directionList[i][j +
							                 1][1] = true;
				                if (dr == WN || dr == WS ||
				                    dr == WE)
					                directionList[i +
					                              1][j][1] = true;
				                if (dl == EW || dl == ES ||
				                    dl == ES)
					                directionList[i +
					                              1][j][0] = true;
				                if (ur == SW || ur == SE ||
				                    ur == SN)
					                directionList[i][j +
					                                 1][0] = true;
				                break;
					case 2: {
						if (dr == NS || dr == NW ||
						    dr == NE)
							directionList[i][j +
							                 1][1] = true;
						if (ur == SW || ur == SE ||
						    ur == SN)
							directionList[i][j +
							                 1][0] = true;
						break;
					}
					case 4: {
						if (dl == ES || dl == EN ||
						    dl == EW)
							directionList[i +
							              1][j][0] = true;
						if (dr == WN || dr == WS ||
						    dr == WE)
							directionList[i +
							              1][j][1] = true;
						if (ur == SW || ur == SN ||
						    ur == SE)
							directionList[i][j +
							                 1][0] = true;
						if (dr == NS || dr == NE ||
						    dr == NW)
							directionList[i][j +
							                 1][1] = true;
						break;
					}
					case 8: {
						if (dl == ES || dl == EN ||
						    dl == EW)
							directionList[i +
							              1][j][0] = true;
						if (dr == WN || dr == WE ||
						    dr == WS)
							directionList[i +
							              1][j][1] = true;
						break;
					}
					case 16: {
						if (dr == SW || dr == SE ||
						    dr == WE)
							directionList[i][j +
							                 1][1] = true;
						if (dr == SE || dr == SN ||
						    dr == EN)
							directionList[i +
							              1][j][1] = true;
						if (dl == NW || dl == NS ||
						    dl == WS)
							directionList[i +
							              1][j][0] = true;
						if (ur == NW || ur == NE ||
						    ur == WE)
							directionList[i][j +
							                 1][0] = true;
						break;
					}
					case 18:
					case 33: {
						directionList[i][j +
						                 1][1] = true;
						directionList[i][j +
						                 1][0] = true;
						directionList[i][j][2] = true;
						break;
					}
					case 20:
					case 65: {
						if (rr != EMPTY)
							directionList[i][j +
							                 1][2] = true;
						directionList[i +
						              1][j][0] = true;
						directionList[i +
						              1][j][1] = true;
						directionList[i][j +
						                 1][0] = true;
						directionList[i][j][0] = true;
						break;
					}
					case 24:
					case 129: {
						directionList[i +
						              1][j][1] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 32: {
						if (dr == SE || dr == SW ||
						    dr == EW)
							directionList[i][j +
							                 1][1] = true;
						if (ur == NW || ur == NE ||
						    ur == WE)
							directionList[i][j +
							                 1][0] = true;
						break;
					}
					case 36: {
						if (ul == NW || ul == SW ||
						    ul == NS)
							directionList[i -
							              1][j][1] = true;
						directionList[i][j +
						                 1][1] = true;
						directionList[i][j +
						                 1][0] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 40:
					case 130: {
						directionList[i][j][0] = true;
						break;
					}
					case 64: {
						if (dl == WN || dl == WS ||
						    dl == NS)
							directionList[i +
							              1][j][0] = true;
						if (dr == EN || dr == ES ||
						    dr == NS)
							directionList[i +
							              1][j][1] = true;
						if (ur == NW || ur == NE ||
						    ur == WE)
							directionList[i][j +
							                 1][0] = true;
						if (dr == SE || dr == SW ||
						    dr == EW)
							directionList[i][j +
							                 1][1] = true;
						break;
					}
					case 66: {
						if (ul == EW || ul == ES ||
						    ul == EN)
							directionList[i -
							              1][j][1] = true;
						directionList[i][j +
						                 1][1] = true;
						directionList[i][j +
						                 1][0] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 72:
					case 132: {
						directionList[i +
						              1][j][0] = true;
						directionList[i +
						              1][j][1] = true;
						directionList[i][j][2] = true;
						break;
					}
					case 128: {
						if (dl == WS || dl == WN ||
						    dl == SN)
							directionList[i +
							              1][j][0] = true;
						if (dr == EN || dr == ES ||
						    dr == NS)
							directionList[i +
							              1][j][1] = true;
						break;
					}
					default:
						break;
				}
			}
		}
	}

	/* collects the moves */
	for (i = iBegin; i <= iEnd; i++) {
		for (j = jBegin; j <= jEnd; j++) {
			/* remove rotation symmetry moves */
			if (rsym && getRowSize() % 2 == 1) {
				int jMiddle = (getColSize() + 1) / 2;
				if (j > jMiddle && i == iEnd) {
					continue;
				}
			}
			if (neighbors[i][j] != 0) {
				ohs_up = 0;
				ohs_down = 0;
				ohs_right = 0;
				ohs_left = 0;
				eks_up = 0;
				eks_down = 0;
				eks_right = 0;
				eks_left = 0;
				up = getAt(i - 1, j);
				down = getAt(i + 1, j);
				left = getAt(i, j - 1);
				right = getAt(i, j + 1);

				if (up == SN || up == SW || up == SE)
					ohs_up = 1;
				else if (up != EMPTY)
					eks_up = 1;
				if (down == NS || down == NW ||
				    down == NE)
					ohs_down = 1;
				else if (down != EMPTY)
					eks_down = 1;
				if (left == EN || left == ES ||
				    left == WE)
					ohs_left = 1;
				else if (left != EMPTY)
					eks_left = 1;
				if (right == WE || right == WS ||
				    right == WN)
					ohs_right = 1;
				else if (right != EMPTY)
					eks_right = 1;

				if (!directionList[i][j][0]) {
					saveState();
					if ((ohs_up + ohs_left > 0)
					    ||
					    (eks_right + eks_down > 0))
						putAt(i, j, NW);
					if ((eks_up + eks_left > 0)
					    ||
					    (ohs_right + ohs_down > 0))
						putAt(i, j, SE);
					if (forcedMove(i - 1, j) &&
					    forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) &&
					    forcedMove(i, j + 1)) {
						getTraxMoveString(i, j,
							moves[movesIndex],
							'/');
						movesIndex++;
					}
					restoreState();
				}
				if (!directionList[i][j][1]) {
					saveState();
					if ((ohs_up + ohs_right > 0)
					    ||
					    (eks_left + eks_down > 0))
						putAt(i, j, NE);
					if ((eks_up + eks_right > 0)
					    ||
					    (ohs_left + ohs_down > 0))
						putAt(i, j, SW);
					if (forcedMove(i - 1, j) &&
					    forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) &&
					    forcedMove(i, j + 1)) {
						getTraxMoveString(i, j,
							moves[movesIndex],
							'\\');
						movesIndex++;
					}
					restoreState();
				}
				if (!directionList[i][j][2]) {
					saveState();
					if ((ohs_up + ohs_down > 0)
					    ||
					    (eks_left + eks_right > 0))
						putAt(i, j, NS);
					if ((eks_up + eks_down > 0)
					    ||
					    (ohs_left + ohs_right > 0))
						putAt(i, j, WE);
					if (forcedMove(i - 1, j) &&
					    forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) &&
					    forcedMove(i, j + 1)) {
						getTraxMoveString(i, j,
							moves[movesIndex],
							'+');
						movesIndex++;
					}
					restoreState();
				}
			}
		}
	}
	return movesIndex;
}