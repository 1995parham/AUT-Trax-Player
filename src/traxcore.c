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
#include <stdio.h>

#include "traxcore.h"
#include "traxmove.h"

#define false 0
#define true 1

static int boardEmpty = 1;
static int wtm;
static int board[8][8];
static int gameOver;
static int numOfTiles;
static int firstRow, lastRow, firstCol, lastCol;

static int whiteCorners, blackCorners;
static int whiteThreats, blackThreats;
static int whiteThreats_save, blackThreats_save;
static int whiteCorners_save, blackCorners_save;

static char *border;

static char *border_save;

static int boardEmpty_save = 1;
static int wtm_save;
static int board_save[8][8];
static int gameOver_save;
static int num_of_tiles_save;
static int firstRow_save, lastRow_save, firstCol_save, lastCol_save;


static int getNumOfTiles(void)
{
	return numOfTiles;
}

void saveState(void)
{
	int i, j;

	wtm_save = wtm;
	boardEmpty_save = boardEmpty;
	gameOver_save = gameOver;
	num_of_tiles_save = numOfTiles;
	firstRow_save = firstRow;
	firstCol_save = firstCol;
	lastRow_save = lastRow;
	lastCol_save = lastCol;
	whiteThreats_save = whiteThreats;
	blackThreats_save = blackThreats;
	whiteCorners_save = whiteCorners;
	blackCorners_save = blackCorners;
	border_save = border;

	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			board_save[i][j] = board[i][j];
}

void restoreState(void)
{
	int i, j;

	wtm = wtm_save;
	boardEmpty = boardEmpty_save;
	gameOver = gameOver_save;
	numOfTiles = num_of_tiles_save;
	firstRow = firstRow_save;
	firstCol = firstCol_save;
	lastRow = lastRow_save;
	lastCol = lastCol_save;
	whiteThreats = whiteThreats_save;
	blackThreats = blackThreats_save;
	whiteCorners = whiteCorners_save;
	blackCorners = blackCorners_save;
	border = border_save;

	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			board[i][j] = board_save[i][j];

}

int getRowSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (lastRow - firstRow));
}

int getColSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (lastCol - firstCol));
}

static int getAt(int row, int col)
{
	if ((row < 1) || (row > 8))
		return EMPTY;
	if ((col < 1) || (col > 8))
		return EMPTY;
	return board[firstRow + row - 1][firstCol + col - 1];
}

static int isBlank(int row, int col)
{
	return (getAt(row, col) == EMPTY);
}

static void putAt(int row, int col, int piece)
{
	if (piece == EMPTY) {
		if (board[firstRow + row - 1][firstCol + col - 1] != EMPTY)
			numOfTiles--;
		board[firstRow + row - 1][firstCol + col - 1] = piece;
		return;
	} else {
		if (boardEmpty) {
			boardEmpty = false;
			firstRow = 7;
			firstCol = 7;
			lastRow = 7;
			lastCol = 7;
			numOfTiles = 1;
			board[firstRow][firstCol] = piece;
			return;
		}
		if (row == 0) {
			firstRow--;
			row++;
		}
		if (col == 0) {
			firstCol--;
			col++;
		}
		if (row > getRowSize()) {
			lastRow += row - getRowSize();
		}
		if (col > getColSize()) {
			lastCol += col - getColSize();
		}
		numOfTiles++;
	}
	board[firstRow + row - 1][firstCol + col - 1] = piece;
}

static void switchPlayer()
{
	switch (wtm) {
		case WHITE:
			wtm = BLACK;
	                break;
		case BLACK:
			wtm = WHITE;
	                break;
		default:
			break;
	                /* This should never happen */
	}
}

static int canMoveDown(void)
{
	return (getRowSize() < 8);
}

static int canMoveRight()
{
	return (getColSize() < 8);
}

static int checkLine(int searchTileRow, int searchTileCol, int winner, int row, int col, char direction, char type)
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
	int tileFound = false;

	newDirection = " uurllrddlrrlllduudrruddu";

	if ((searchTileRow == row) && (searchTileCol == col)) {
		tileFound = true;
	}
	for (; ;) {
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
		if ((searchTileRow == row) && (searchTileCol == col)) {
			tileFound = true;
		}
		if ((type == 'h') && (col == 9)) {
			return !winner || tileFound;
		}
		if ((type == 'v') && (row == 9)) {
			return !winner || tileFound;
		}
		if ((row == start_row) && (col == start_col)) {
			return type == 'l' && (!winner || tileFound);
		}
	}
}

int isGameOver(void)
{
	int WhiteWins = false, BlackWins = false;

	if (gameOver != NOPLAYER)
		return gameOver;
	if (numOfTiles < 4) {
		gameOver = NOPLAYER;
		return gameOver;
	}

	/* check loop wins */
	int i, j;
	for (i = 1; i < 8; i++) {
		for (j = 1; j < 8; j++) {
			switch (getAt(i, j)) {
				case NW:
					if (checkLine(0, 0, false, i, j, 'u', 'l')) BlackWins = true;
			                break;
				case SE:
					if (checkLine(0, 0, false, i, j, 'u', 'l')) WhiteWins = true;
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
		gameOver = whoDidLastMove();
		return gameOver;
	}
	if (WhiteWins) {
		gameOver = WHITE;
		return gameOver;
	}
	if (BlackWins) {
		gameOver = BLACK;
		return gameOver;
	}
	return NOPLAYER;
}

int makeMove(const char *move)
{
	int col, row, neighbor;
	char direction;
	int ohs_up = 0, ohs_down = 0, ohs_right = 0, ohs_left = 0,
		eks_up = 0, eks_down = 0, eks_right = 0, eks_left = 0;

	if (gameOver != NOPLAYER); /* Game is over */
	if (strlen(move) != 3); /* invalid move */

	/* move = move.toUpperCase(); */

	if (boardEmpty) {
		if (!strcmp(move, "@0/")) {
			putAt(1, 1, NW);
			border = NULL;
			switchPlayer();
			whiteCorners = 1;
			blackCorners = 1;
			return 0;
		}
		if (!strcmp(move, "@0+")) {
			putAt(1, 1, NS);
			border = NULL;
			switchPlayer();
			return 0;
		}
	}

	col = move[0] - '@';
	row = move[1] - '0';

	direction = move[2];

	if (col == 0 && row == 0)
		return -1; /* No neighbours */
	if ((row == 0) && (!canMoveDown()))
		return -1; /* illegal row */
	if ((col == 0) && (!canMoveRight()))
		return -1; /* illegal column */
	if (!isBlank(row, col))
		return -1; /* occupy */

	int up = getAt(row - 1, col), down = getAt(row + 1, col), left = getAt(row, col - 1), right = getAt(row,
		col + 1);

	if (up == SN || up == SE || up == SW) ohs_up = 1;
	if (up == EW || up == NW || up == NE) eks_up = 1;
	if (down == NS || down == NE || down == NW) ohs_down = 1;
	if (down == EW || down == SW || down == SE) eks_down = 1;
	if (left == EN || left == ES || left == EW) ohs_left = 1;
	if (left == WS || left == WN || left == NS) eks_left = 1;
	if (right == WN || right == WE || right == WS) ohs_right = 1;
	if (right == ES || right == NS || right == EN) eks_right = 1;
	neighbor = ohs_up + (2 * ohs_down) + (4 * ohs_left) + (8 * ohs_right)
	           + (16 * eks_up) + (32 * eks_down) + (64 * eks_left) + (128 * eks_right);

	switch (neighbor) {
		case 0:
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
					break;
			                /* This should never happen */
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
					break;
			                /* This should never happen */
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
	/* note that switchPlayer() _must_ come before isGameOver() */
	switchPlayer();
	/* updates the gameOver attribute */
	isGameOver();
	return 0;

}

int whoToMove(void)
{
	return wtm;
}

int whoDidLastMove(void)
{
	if (boardEmpty)
		return NOPLAYER;
	switch (wtm) {
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

static int forcedMove(int brow, int bcol)
{
	if (!isBlank(brow, bcol)) return true;
	if ((brow < 1) || (brow > 8) || (bcol < 1) || (bcol > 8)) return true;

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

	if ((white > 2) || (black > 2)) { return false; } // Illegal filled cave
	if ((white < 2) && (black < 2)) { return true; } // Done

	int piece = EMPTY;
	if (white == 2) {
		switch (white_up + 2 * white_down + 4 * white_left + 8 * white_right) {
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
		switch (black_up + 2 * black_down + 4 * black_left + 8 * black_right) {
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

static int isLeftRightMirror()
{
	int piece, i, j, j2;

	for (i = 1; i <= getRowSize(); i++) {
		j2 = getColSize();
		for (j = 1; j <= ((getColSize() + 1) / 2); j++) {
			piece = getAt(i, j);
			switch (getAt(i, j2)) {
				case NW:
					if (piece != NE)
						return false;
			                break;
				case NE:
					if (piece != NW)
						return false;
			                break;
				case SW:
					if (piece != SE)
						return false;
			                break;
				case SE:
					if (piece != SW)
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
	}
	return true;
}

static int isUpDownMirror()
{
	int piece, i, j, i2;

	i2 = getRowSize();
	for (i = 1; i <= ((getRowSize() + 1) / 2); i++) {
		for (j = 1; j <= getColSize(); j++) {
			piece = getAt(i, j);
			switch (getAt(i2, j)) {
				case NW:
					if (piece != SW)
						return false;
			                break;
				case NE:
					if (piece != SE)
						return false;
			                break;
				case SW:
					if (piece != NW)
						return false;
			                break;
				case SE:
					if (piece != NE)
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
		}
		i2--;
	}
	return true;
}

static int isRotateMirror()
{
	int i, j, piece, i2, j2;

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


int uniqueMoves(int remove_mirror_moves, char moves[][256])
{
	/*
	 * complex throw away a lot of equal moves
	 * and symmetries (hopefully)
	*/

	int movesIndex = 0;

	int i, j, k;
	int dl, dr, ur, rr, dd;
	/* which neighbors - default all values 0 */
	int neighbors[10][10];
	int directionList[10][10][3];
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

	int lrsym, udsym, rsym;

	if (gameOver == NOPLAYER) {
		return 0;
	}

	/* empty board only two moves */
	if (boardEmpty) {
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
		                strcpy(moves[movesIndex], "E1+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "E1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "E1\\");
		                movesIndex++;
		                if (!remove_mirror_moves) {
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
		                }
		                break;
			case NS:
				strcpy(moves[movesIndex], "@1+");
		                movesIndex++;
		                strcpy(moves[movesIndex], "@1/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0/");
		                movesIndex++;
		                strcpy(moves[movesIndex], "A0+");
		                movesIndex++;
		                if (!remove_mirror_moves) {
			                strcpy(moves[movesIndex], "@1\\");
			                movesIndex++;
			                strcpy(moves[movesIndex], "A0\\");
			                movesIndex++;
			                strcpy(moves[movesIndex], "E1/");
			                movesIndex++;
			                strcpy(moves[movesIndex], "E1\\");
			                movesIndex++;
			                strcpy(moves[movesIndex], "A2/");
			                movesIndex++;
			                strcpy(moves[movesIndex], "A2\\");
			                movesIndex++;
		                }
		                break;
			default:
				/* This should never happen */
				break;
		}
		return movesIndex;
	}

	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			for (k = 0; k < 3; k++)
				directionList[i][j][k] = false;

	lrsym = isLeftRightMirror();
	udsym = isUpDownMirror();
	rsym = isRotateMirror();
	iBegin = (canMoveDown()) ? 0 : 1;
	jBegin = (canMoveRight()) ? 0 : 1;
	iEnd = (getRowSize() < 8) ? getRowSize() + 1 : 8;
	jEnd = (getColSize() < 8) ? getColSize() + 1 : 8;
	if (remove_mirror_moves) {
		if (lrsym)
			jEnd = (getColSize() + 1) / 2;
		if (rsym || udsym)
			iEnd = (getRowSize() + 1) / 2;
	}

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

				if (down == NS || down == NW || down == NE) {
					ohs_down = 1;
				} else if (down != EMPTY) {
					eks_down = 1;
				}

				if (left == EN || left == ES || left == WE)
					ohs_left = 1;
				else if (left != EMPTY)
					eks_left = 1;

				if (right == WE || right == WS || right == WN)
					ohs_right = 1;
				else if (right != EMPTY)
					eks_right = 1;

				neighbors[i][j] = ohs_up + 2 * ohs_down + 4 * ohs_left
				                  + 8 * ohs_right + 16 * eks_up + 32 * eks_down + 64
				                                                                  * eks_left +
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
				getAt(i - 1, j - 1);
				rr = getAt(i, j + 2);
				dd = getAt(i + 2, j);
				switch (neighbors[i][j]) {
					case 1: {
						if (ur == SW || ur == SE || ur == SN)
							directionList[i][j + 1][0] = true;
						if (dr == NS || dr == NW || dr == NE)
							directionList[i][j + 1][1] = true;
						if (rr == WS || rr == WE || rr == WN)
							directionList[i][j + 1][2] = true;
						if (dr == WN || dr == WS || dr == WE)
							directionList[i + 1][j][1] = true;
						if (dl == EW || dl == ES || dl == ES)
							directionList[i + 1][j][0] = true;
						break;
					}
					case 2: {
						if (dr == NS || dr == NW || dr == NE)
							directionList[i][j + 1][1] = true;
						if (ur == SW || ur == SE || ur == SN)
							directionList[i][j + 1][0] = true;
						break;
					}
					case 4: {
						if (dl == ES || dl == EN || dl == EW)
							directionList[i + 1][j][0] = true;
						if (dr == WN || dr == WS || dr == WE)
							directionList[i + 1][j][1] = true;
						if (ur == SW || ur == SN || ur == SE)
							directionList[i][j + 1][0] = true;
						if (dr == NS || dr == NE || dr == NW)
							directionList[i][j + 1][1] = true;
						if (dd == NW || dd == NE || dd == NS)
							directionList[i + 1][j][2] = true;
						break;
					}
					case 8: {
						if (dl == ES || dl == EN || dl == EW)
							directionList[i + 1][j][0] = true;
						if (dr == WN || dr == WE || dr == WS)
							directionList[i + 1][j][1] = true;
						if (dd == NW || dd == NS || dd == NE)
							directionList[i + 1][j][2] = true;
						break;
					}
					case 16: {
						if (ur == NW || ur == NE || ur == WE)
							directionList[i][j + 1][0] = true;
						if (dr == SW || dr == SE || dr == WE)
							directionList[i][j + 1][1] = true;
						if (rr == NE || rr == NS || rr == SE)
							directionList[i][j + 1][2] = true;
						if (dr == SE || dr == SN || dr == EN)
							directionList[i + 1][j][1] = true;
						if (dl == NW || dl == NS || dl == WS)
							directionList[i + 1][j][0] = true;
						break;
					}
					case 18:
					case 33: {
						if (rr != EMPTY) directionList[i][j + 1][2] = true;
						if (dr != EMPTY || ur != EMPTY) {
							directionList[i][j + 1][1] = true;
							directionList[i][j + 1][0] = true;
						}
						directionList[i][j][2] = true;
						break;
					}
					case 20:
						if (rr != EMPTY)
							directionList[i][j + 1][2] = true;
				                directionList[i + 1][j][0] = true;
				                directionList[i][j + 1][0] = true;
				                directionList[i][j][0] = true;
				                if (dd == NE || dd == NW || dd == NS)
					                directionList[i + 1][j][2] = true;
				                break;
					case 65: {
						if (rr != EMPTY)
							directionList[i][j + 1][2] = true;
						directionList[i + 1][j][0] = true;
						directionList[i][j + 1][0] = true;
						directionList[i][j][0] = true;
						if (dd == SW || dd == SE || dd == WE)
							directionList[i + 1][j][2] = true;
						break;
					}
					case 24:
					case 129: {
						directionList[i + 1][j][1] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 32: {
						if (dr == SE || dr == SW || dr == EW)
							directionList[i][j + 1][1] = true;
						if (ur == NW || ur == NE || ur == WE)
							directionList[i][j + 1][0] = true;
						break;
					}
					case 36: {
						directionList[i][j + 1][1] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 40:
					case 130: {
						directionList[i][j][0] = true;
						break;
					}
					case 64: {
						if (dl == WN || dl == WS || dl == NS)
							directionList[i + 1][j][0] = true;
						if (dr == EN || dr == ES || dr == NS)
							directionList[i + 1][j][1] = true;
						if (ur == NW || ur == NE || ur == WE)
							directionList[i][j + 1][0] = true;
						if (dr == SE || dr == SW || dr == EW)
							directionList[i][j + 1][1] = true;
						if (dd == SW || dd == SE || dd == WE)
							directionList[i + 1][j][2] = true;
						break;
					}
					case 66: {
						directionList[i][j + 1][1] = true;
						directionList[i][j][1] = true;
						break;
					}
					case 72:
					case 132: {
						if (dl != EMPTY || dr != EMPTY) {
							directionList[i + 1][j][0] = true;
							directionList[i + 1][j][1] = true;
						}
						if (dd != EMPTY) directionList[i + 1][j][2] = true;
						directionList[i][j][2] = true;
						break;
					}
					case 128: {
						if (dl == WS || dl == WN || dl == SN)
							directionList[i + 1][j][0] = true;
						if (dr == EN || dr == ES || dr == NS)
							directionList[i + 1][j][1] = true;
						break;
					}
					default:
						/* This should never happen */
						break;
				}
			}
		}
	}

	if (remove_mirror_moves) {
		/* remove left-right symmetry moves */
		if (lrsym && getColSize() % 2 == 1) {
			for (i = iBegin; i <= iEnd; i++) {
				directionList[i][jEnd][0] = true;
			}
		}
		/* remove up-down symmetry moves */
		if (udsym && getRowSize() % 2 == 1) {
			for (j = jBegin; j <= jEnd; j++) {
				directionList[iEnd][j][1] = true;
			}
		}
	}

	/* collects the moves */
	for (i = iBegin; i <= iEnd; i++) {
		for (j = jBegin; j <= jEnd; j++) {
			// remove rotation symmetry moves
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
				if (down == NS || down == NW || down == NE)
					ohs_down = 1;
				else if (down != EMPTY)
					eks_down = 1;
				if (left == EN || left == ES || left == WE)
					ohs_left = 1;
				else if (left != EMPTY)
					eks_left = 1;
				if (right == WE || right == WS || right == WN)
					ohs_right = 1;
				else if (right != EMPTY)
					eks_right = 1;

				if (!directionList[i][j][0]) {
					saveState();
					if ((ohs_up + ohs_left > 0)
					    || (eks_right + eks_down > 0))
						putAt(i, j, NW);
					if ((eks_up + eks_left > 0)
					    || (ohs_right + ohs_down > 0))
						putAt(i, j, SE);
					if (forcedMove(i - 1, j) && forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) && forcedMove(i, j + 1)) {
						getTraxString(i, j, moves[movesIndex], '/');
						movesIndex++;
					}
					restoreState();
				}
				if (!directionList[i][j][1]) {
					saveState();
					if ((ohs_up + ohs_right > 0)
					    || (eks_left + eks_down > 0))
						putAt(i, j, NE);
					if ((eks_up + eks_right > 0)
					    || (ohs_left + ohs_down > 0))
						putAt(i, j, SW);
					if (forcedMove(i - 1, j) && forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) && forcedMove(i, j + 1)) {
						getTraxString(i, j, moves[movesIndex], '\\');
						movesIndex++;
					}
					restoreState();
				}
				if (!directionList[i][j][2]) {
					saveState();
					if ((ohs_up + ohs_down > 0)
					    || (eks_left + eks_right > 0))
						putAt(i, j, NS);
					if ((eks_up + eks_down > 0)
					    || (ohs_left + ohs_right > 0))
						putAt(i, j, WE);
					if (forcedMove(i - 1, j) && forcedMove(i + 1, j)
					    && forcedMove(i, j - 1) && forcedMove(i, j + 1)) {
						getTraxString(i, j, moves[movesIndex], '+');
						movesIndex++;
					}
					restoreState();
				}
			}
		}
	}
	return movesIndex;
}