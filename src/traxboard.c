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
#include "string.h"
#include "traxboard.h"

#define false 0
#define true 1

static int boardEmpty;
static int wtm;
static int **board;
static int gameover;
static int num_of_tiles;
static int firstrow, lastrow, firstcol, lastcol;
static int whiteCorners, blackCorners;
static int whiteThreats, blackThreats;
static int whiteThreats_save, blackThreats_save;
static int whiteCorners_save, blackCorners_save;

static char *border;
static char *border_save;

static int boardEmpty_save;
static int wtm_save;
static int **board_save;
static int gameover_save;
static int num_of_tiles_save;
static int firstrow_save, lastrow_save, firstcol_save, lastcol_save;


/*
static String[][] col_row_array;
*/
/*
    static
    {
        StringBuffer str;
        col_row_array = new String[9][9];
        for (char i = '@'; i <= 'H'; i++) {
            for (char j = '0'; j <= '8'; j++) {
                str = new StringBuffer();
                str.append(i);
                str.append(j);
                col_row_array[i - '@'][j - '0'] = new String(str);
            }
        }
    }
*/

int blank(int piece)
{
	return (piece == EMPTY);
}

int getNumOfTiles(void)
{
	return num_of_tiles;
}


/*
static rotate( tb)
{
        Traxboard result=new Traxboard(tb);
        for (int i=0; i<17; i++) {
            for (int j=0; j<17; j++) {
                switch (tb.board[16-j][i]) {
                    case NS:
                        result.board[i][j]=WE;
                        break;
                    case WE:
                        result.board[i][j]=NS;
                        break;
                    case EMPTY:
                        result.board[i][j]=EMPTY;
                        break;
                    case NW:
                        result.board[i][j]=NE;
                        break;
                    case NE:
                        result.board[i][j]=SE;
                        break;
                    case SE:
                        result.board[i][j]=SW;
                        break;
                    case SW:
                        result.board[i][j]=NW;
                        break;
                    default:
                        // This should never happen
                        throw new RuntimeException("This should never happen.");
                }
            }
        }
        result.setCorners();
        result.border=null;
        return result;
    }

    public int getNumberOfWhiteThreats() {
        if (whiteThreats==-1) count2times();
        return whiteThreats;
    }

    public int getNumberOfBlackThreats() {
        if (blackThreats==-1) count2times();
        return blackThreats;
    }

    public int getNumberOfWhiteCorners() {
        if (whiteCorners==-1) count2times();
        return whiteCorners;
    }

    public int getNumberOfBlackCorners() {
        if (blackCorners==-1) count2times();
        return blackCorners;
    }

    private void count2times() {
        Traxboard t_copy=new Traxboard(rotate(this));

        this.count();
        t_copy.count();

        this.whiteCorners=Math.max(this.whiteCorners,t_copy.whiteCorners);
        this.whiteThreats=Math.max(this.whiteThreats,t_copy.whiteThreats);
        this.blackCorners=Math.max(this.blackCorners,t_copy.blackCorners);
        this.blackThreats=Math.max(this.blackThreats,t_copy.blackThreats);
    }

    private void setCorners()
    {
        firstrow=-1;
        firstcol=-1;
        lastcol=-1;
        lastrow=-1;
        for (int i=0; i<17; i++) {
            for (int j=0; j<17; j++) {
                if ((firstrow<0) && (board[i][j]!=EMPTY)) firstrow=i;
                if ((lastrow<0) && (board[16-i][j]!=EMPTY)) lastrow=16-i;
                if ((firstcol<0) && (board[j][i]!=EMPTY)) firstcol=i;
                if ((lastcol<0) && (board[j][16-i]!=EMPTY)) lastcol=16-i;
            }
        }
    }
*/
static void saveState(void)
{
	int i, j;

	wtm_save = wtm;
	boardEmpty_save = boardEmpty;
	gameover_save = gameover;
	num_of_tiles_save = num_of_tiles;
	firstrow_save = firstrow;
	firstcol_save = firstcol;
	lastrow_save = lastrow;
	lastcol_save = lastcol;
	whiteThreats_save = whiteThreats;
	blackThreats_save = blackThreats;
	whiteCorners_save = whiteCorners;
	blackCorners_save = blackCorners;
	border_save = border;

	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			board_save[i][j] = board[i][j];
}

static void restoreState(void)
{
	int i, j;

	wtm = wtm_save;
	boardEmpty = boardEmpty_save;
	gameover = gameover_save;
	num_of_tiles = num_of_tiles_save;
	firstrow = firstrow_save;
	firstcol = firstcol_save;
	lastrow = lastrow_save;
	lastcol = lastcol_save;
	whiteThreats = whiteThreats_save;
	blackThreats = blackThreats_save;
	whiteCorners = whiteCorners_save;
	blackCorners = blackCorners_save;
	border = border_save;

	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			board[i][j] = board_save[i][j];

}

/*
Traxboard()
    {
        int i, j;

        wtm = WHITE;
        gameover = NOPLAYER;
        num_of_tiles = 0;
        whiteCorners=0;
        blackCorners=0;
        whiteThreats=0;
        blackThreats=0;
        border="";

        board = new int[17][17];
        board_save = new int[17][17];
        for (i = 0; i < 17; i++)
            for (j = 0; j < 17; j++)
                board[i][j] = EMPTY;
        boardEmpty = true;

}

    public Traxboard(Traxboard org)
    {
        int i, j;

        wtm = org.wtm;
        gameover = org.gameover;
        num_of_tiles = org.num_of_tiles;
        board = new int[17][17];
        board_save = new int[17][17];
        for (i = 0; i < 17; i++) {
            for (j = 0; j < 17; j++) {
                this.board[i][j] = org.board[i][j];
                this.board_save[i][j] = org.board_save[i][j];
            }
        }
        firstrow = org.firstrow;
        firstcol = org.firstcol;
        lastrow = org.lastrow;
        lastcol = org.lastcol;
        firstrow_save = org.firstrow_save;
        firstcol_save = org.firstcol_save;
        lastrow_save = org.lastrow_save;
        lastcol_save = org.lastcol_save;
        boardEmpty = org.boardEmpty;
        whiteThreats_save = org.whiteThreats_save;
        blackThreats_save = org.blackThreats_save;
        whiteThreats = org.whiteThreats;
        blackThreats = org.blackThreats;
        whiteCorners_save = org.whiteCorners;
        blackCorners_save = org.blackCorners;
        whiteCorners = org.whiteCorners;
        blackCorners = org.blackCorners;
        border = org.border;
    }
*/

int getRowSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (lastrow - firstrow));
}

int getColSize(void)
{
	return ((getNumOfTiles() == 0) ? 0 : 1 + (lastcol - firstcol));
}

int getAt(int row, int col)
{
	if ((row < 1) || (row > 8))
		return EMPTY;
	if ((col < 1) || (col > 8))
		return EMPTY;
	return board[firstrow + row - 1][firstcol + col - 1];
}

int isBlank(int row, int col)
{
	return (getAt(row, col) == EMPTY);
}

void putAt(int row, int col, int piece)
{
	if (piece == EMPTY) {
		if (board[firstrow + row - 1][firstcol + col - 1] != EMPTY)
			num_of_tiles--;
		board[firstrow + row - 1][firstcol + col - 1] = piece;
		return;
	} else {
		if (boardEmpty) {
			boardEmpty = false;
			firstrow = 7;
			firstcol = 7;
			lastrow = 7;
			lastcol = 7;
			num_of_tiles = 1;
			board[firstrow][firstcol] = piece;
			return;
		}
		if (row == 0) {
			firstrow--;
			row++;
		}
		if (col == 0) {
			firstcol--;
			col++;
		}
		if (row > getRowSize()) {
			lastrow += row - getRowSize();
		}
		if (col > getColSize()) {
			lastcol += col - getColSize();
		}
		num_of_tiles++;
	}
	board[firstrow + row - 1][firstcol + col - 1] = piece;
}

void makeMove(char *move)
{
	int oldNotation;
	int col, row, neighbor;
	char direction;
	int ohs_up = 0, ohs_down = 0, ohs_right = 0, ohs_left = 0,
		eks_up = 0, eks_down = 0, eks_right = 0, eks_left = 0;

	if (gameover != NOPLAYER); /* Game is over */
	if (strlen(move) != 3); /* invalid move */

	/* move = move.toUpperCase(); */

	if (boardEmpty) {
		if (!strcmp(move, "@0/")) {
			putAt(1, 1, NW);
			border = NULL;
			switchPlayer();
			whiteCorners = 1;
			blackCorners = 1;
			return;
		}
		if (!strcmp(move, "@0+")) {
			putAt(1, 1, NS);
			border = NULL;
			switchPlayer();
			return;
		}
	}

	col = move[0] - '@';
	row = move[1] - '0';

	direction = move[2];

	if (col == 0 && row == 0); /* No neighbours */
	if ((row == 0) && (!canMoveDown())); /* illegal row */
	if ((col == 0) && (!canMoveRight())); /* illegal column */
	if (!isBlank(row, col)); /* occupy */

	saveState();
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
				case 'R':
					putAt(row, col, SE);
			                break;
				case '\\':
				case 'L':
				case 'C':
					putAt(row, col, SW);
			                break;
				case '+':
				case 'S':
				case 'U':
				case 'D':
				default:
					break;
			                /* This should never happen */
			}
	                break;
		case 20:
			switch (direction) {
				case '/':
				case 'L':
				case 'U':
					break;
				case '\\':
				case 'C':
				case 'R':
				case 'D':
					putAt(row, col, WS);
			                break;
				case '+':
				case 'S':
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
				case 'L':
				case 'C':
				case 'D':
					putAt(row, col, SE);
			                break;
				case 'U':
				case '\\':
				case 'R':
					break;
				case 'S':
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
				case 'R':
					putAt(row, col, NW);
			                break;
				case 'D':
				case 'U':
				case 'C':
					break;
				case '\\':
				case 'L':
					putAt(row, col, NE);
			                break;
				case 'S':
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
				case 'L':
					putAt(row, col, NW);
			                break;
				case 'R':
				case '\\':
					putAt(row, col, NE);
			                break;
				case 'C':
				case 'S':
				case '+':
				case 'D':
				case 'U':
					break;
				default:
					break;
			                /* This should never happen */
			}
	                break;
		case 36:
			if (direction == '/') putAt(row, col, NW);
	                if (direction == '+') putAt(row, col, WE);
	                if (direction == 'S') putAt(row, col, WE);
	                if (direction == 'C') putAt(row, col, WN);
	                if (direction == 'R') putAt(row, col, WN);
	                if (direction == 'U') putAt(row, col, WN);
	                break;
		case 40:
			if (direction == '\\') putAt(row, col, EN);
	                if (direction == '+') putAt(row, col, EW);
	                if (direction == 'S') putAt(row, col, WE);
	                if (direction == 'C') putAt(row, col, NE);
	                if (direction == 'L') putAt(row, col, NE);
	                if (direction == 'U') putAt(row, col, NE);
	                break;
		case 64:
			if (direction == '/') putAt(row, col, ES);
	                if (direction == '\\') putAt(row, col, EN);
	                if (direction == '+') putAt(row, col, NS);
	                if (direction == 'S') putAt(row, col, NS);
	                if (direction == 'U') putAt(row, col, SE);
	                if (direction == 'D') putAt(row, col, NE);
	                break;
		case 65:
			if (direction == '\\') putAt(row, col, NE);
	                if (direction == '+') putAt(row, col, NS);
	                if (direction == 'S') putAt(row, col, NS);
	                if (direction == 'C') putAt(row, col, NE);
	                if (direction == 'R') putAt(row, col, NE);
	                if (direction == 'D') putAt(row, col, NE);
	                break;
		case 66:
			if (direction == '/') putAt(row, col, SE);
	                if (direction == '+') putAt(row, col, SN);
	                if (direction == 'S') putAt(row, col, SN);
	                if (direction == 'C') putAt(row, col, SE);
	                if (direction == 'R') putAt(row, col, SE);
	                if (direction == 'U') putAt(row, col, SE);
	                break;
		case 72:
			if (direction == '/') putAt(row, col, ES);
	                if (direction == '\\') putAt(row, col, EN);
	                if (direction == 'U') putAt(row, col, NE);
	                if (direction == 'D') putAt(row, col, SE);
	                break;
		case 128:
			if (direction == '/') putAt(row, col, WN);
	                if (direction == '\\') putAt(row, col, WS);
	                if (direction == '+') putAt(row, col, NS);
	                if (direction == 'S') putAt(row, col, NS);
	                if (direction == 'U') putAt(row, col, WS);
	                if (direction == 'D') putAt(row, col, WN);
	                break;
		case 129:
			if (direction == '/') putAt(row, col, NW);
	                if (direction == '+') putAt(row, col, NS);
	                if (direction == 'S') putAt(row, col, NS);
	                if (direction == 'C') putAt(row, col, NW);
	                if (direction == 'L') putAt(row, col, NW);
	                if (direction == 'D') putAt(row, col, NW);
	                break;
		case 130:
			if (direction == '\\') putAt(row, col, SW);
	                if (direction == '+') putAt(row, col, SN);
	                if (direction == 'S') putAt(row, col, SN);
	                if (direction == 'C') putAt(row, col, SW);
	                if (direction == 'L') putAt(row, col, SW);
	                if (direction == 'U') putAt(row, col, SW);
	                break;
		case 132:
			if (direction == '/') putAt(row, col, WN);
	                if (direction == '\\') putAt(row, col, WS);
	                if (direction == 'U') putAt(row, col, WN);
	                if (direction == 'D') putAt(row, col, WS);
	                break;
		default:
			break;
	                /* This should never happen */
	}
	/* note that switchPlayer() _must_ come before isGameOver() */
	switchPlayer();
	isGameOver(); // updates the gameOver attribute

}

void switchPlayer()
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

int checkLine(int searchTileRow, int searchTileCol, int winner, int row, int col, char direction, char type)
{
	// type can be _h_orizontal , _v_ertical, _l_oop

	int start_row = row;
	int start_col = col;
	int ix = 0;
	const char *newdir;
	int tileFound = false;

	newdir = " uurllrddlrrlllduudrruddu";

	if ((searchTileRow == row) && (searchTileCol == col)) {
		tileFound = true;
	}
	for (; ;) {
		if (isBlank(row, col))
			return false; // no line starts with a empty space
		// or we are out of range
		switch (direction) {
			case 'u':
				// newdir's first line
				ix = 0;
		                break;
			case 'd':
				// newdir's second line
				ix = 6;
		                break;
			case 'l':
				// newdir's third line
				ix = 12;
		                break;
			case 'r':
				// newdir's fourth line
				ix = 18;
		                break;
			default:
				break;
		}
		ix += getAt(row, col);
		direction = newdir[ix];
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

int isGameOver()
{
	int WhiteWins = false, BlackWins = false;
	int sp;

	if (gameover != NOPLAYER) return gameover;
	if (num_of_tiles < 4) {
		gameover = NOPLAYER;
		return gameover;
	}

	// check for line win.
	// check left-right line
	if (getColSize() == 8) {
		// check left-right line
		for (int row = 1; row <= 8; row++) {
			if (checkLine(0, 0, false, row, 1, 'r', 'h')) {
				// Line win
				sp = getAt(row, 1);
				if (sp == NS || sp == NE || sp == ES)
					BlackWins = true;
				else
					WhiteWins = true;
			}
		}
	}
	// check up-down line
	if (getRowSize() == 8) {
		for (int col = 1; col <= 8; col++) {
			if (checkLine(0, 0, false, 1, col, 'd', 'v')) {
				// Line win
				sp = getAt(1, col);
				if (sp == WE || sp == WS || sp == SE)
					BlackWins = true;
				else
					WhiteWins = true;
			}
		}
	}

	// if (need_loop_check==true) {
	// Now check loop wins
	for (int i = 1; i < 8; i++) {
		for (int j = 1; j < 8; j++) {
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
	// }

	if (WhiteWins && BlackWins) {
		gameover = whoDidLastMove();
		return gameover;
	}
	if (WhiteWins) {
		gameover = WHITE;
		return gameover;
	}
	if (BlackWins) {
		gameover = BLACK;
		return gameover;
	}
	return NOPLAYER;
}


int whoToMove()
{
	return wtm;
}

int whoDidLastMove()
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
	                // This should never happen
	}
}

int winnerTile(int row, int col)
{
	if (isGameOver() == NOPLAYER)
		return false;
	if (isGameOver() == DRAW)
		return false;

	if (getColSize() == 8) {
		// check left-right line
		for (int r = 1; r <= 8; r++) {
			if (checkLine(row, col, true, r, 1, 'r', 'h')) return true;
		}
	}
	// check up-down line
	if (getRowSize() == 8) {
		for (int c = 1; c <= 8; c++) {
			if (checkLine(row, col, true, 1, c, 'd', 'v')) return true;
		}
	}
	for (int i = 1; i < 8; i++) {
		for (int j = 1; j < 8; j++) {
			switch (getAt(i, j)) {
				case NW:
				case SE:
					if (checkLine(row, col, true, i, j, 'u', 'l')) return true;
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

	return false;
}


int canMoveDown(void)
{
	return (getRowSize() < 8);
}

int canMoveRight()
{
	return (getColSize() < 8);
}

int forcedMove(int brow, int bcol)
{
	if (!isBlank(brow, bcol)) return true;
	if ((brow < 1) || (brow > 8) || (bcol < 1) || (bcol > 8)) return true;

	int up = getAt(brow - 1, bcol);
	int down = getAt(brow + 1, bcol);
	int left = getAt(brow, bcol - 1);
	int right = getAt(brow, bcol + 1);

	// boolean result=true;
	int neighbors = 0;

	if (!blank(up)) neighbors++;
	if (!blank(down)) neighbors++;
	if (!blank(left)) neighbors++;
	if (!blank(right)) neighbors++;

	if (neighbors < 2) return true; // Less than two pieces bordering

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
		}
	} else { // right==2
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
		}
	}
	putAt(brow, bcol, piece);
	if (!forcedMove(brow - 1, bcol)) { return false; }
	if (!forcedMove(brow + 1, bcol)) { return false; }
	if (!forcedMove(brow, bcol - 1)) { return false; }
	return forcedMove(brow, bcol + 1);
}

void updateLine(char colour, char entry, int row, int col)
{
	int theNum;

	while (true) {
		theNum = 0;
		if (colour == 'w') theNum = 1024;
		switch (entry) {
			case 'w':
				theNum += 512;
		                break;
			case 'e':
				theNum += 256;
		                break;
			case 's':
				theNum += 128;
		                break;
			case 'n':
				theNum += 64;
		                break;
			default:
				break;
		                // This should never happen
		}
		switch (getAt(row, col)) {
			case NS:
				theNum += 32;
		                break;
			case WE:
				theNum += 16;
		                break;
			case NW:
				theNum += 8;
		                break;
			case NE:
				theNum += 4;
		                break;
			case SW:
				theNum += 2;
		                break;
			case SE:
				theNum += 1;
		                break;
			default:
				break;
		                // This should never happen
		}
		switch (theNum) {
			case 1024 + 512 + 16:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                break;
			case 1024 + 512 + 8:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                entry = 's';
		                break;
			case 1024 + 512 + 2:
				if (getAt(row + 1, col) == EMPTY) return;
		                row++;
		                entry = 'n';
		                break;
			case 1024 + 256 + 16:
				if (getAt(row, col - 1) == EMPTY) return;
		                col--;
		                break;
			case 1024 + 256 + 4:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                entry = 's';
		                break;
			case 1024 + 256 + 1:
				if (getAt(row + 1, col) == EMPTY) return;
		                row++;
		                entry = 'n';
		                break;
			case 1024 + 128 + 32:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                break;
			case 1024 + 128 + 2:
				if (getAt(row, col - 1) == EMPTY) return;
		                col--;
		                entry = 'e';
		                break;
			case 1024 + 128 + 1:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                entry = 'w';
		                break;
			case 1024 + 64 + 32:
				if (getAt(row + 1, col) == EMPTY) return;
		                row++;
		                break;
			case 1024 + 64 + 8:
				if (getAt(row, col - 1) == EMPTY) return;
		                col--;
		                entry = 'e';
		                break;
			case 1024 + 64 + 4:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                entry = 'w';
		                break;
			case 512 + 32:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                break;
			case 512 + 4:
				if (getAt(row + 1, col) == EMPTY) return;
		                row++;
		                entry = 'n';
		                break;
			case 512 + 1:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                entry = 's';
		                break;
			case 256 + 32:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                break;
			case 256 + 8:
				if (getAt(row, col + 1) == EMPTY) return;
		                row++;
		                entry = 'n';
		                break;
			case 256 + 2:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                entry = 's';
		                break;
			case 128 + 16:
				if (getAt(row - 1, col) == EMPTY) return;
		                row--;
		                break;
			case 128 + 8:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                entry = 'w';
		                break;
			case 128 + 4:
				if (getAt(row, col - 1) == EMPTY) return;
		                col--;
		                entry = 'e';
		                break;
			case 64 + 16:
				if (getAt(row + 1, col) == EMPTY) return;
		                row++;
		                break;
			case 64 + 2:
				if (getAt(row, col + 1) == EMPTY) return;
		                col++;
		                entry = 'w';
		                break;
			case 64 + 1:
				if (getAt(row, col - 1) == EMPTY) return;
		                col--;
		                entry = 'e';
		                break;
			default:
				break;
		                /* This should never happen */
		}
	}
}
