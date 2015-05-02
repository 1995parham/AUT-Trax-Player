/*
 * In The Name Of God
 * ========================================
 * [] File Name : traxmove.h
 *
 * [] Creation Date : 01-05-2015
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2015 Parham Alvani.
*/
#ifndef TRAXMOVE_H
#define TRAXMOVE_H

void getTraxMoveString(int i, int j, char *aMove, char direction);

/*
 * convert a trax move string into
 * i (row number), j (column number) and direction (tile type)
*/
void getTraxMoveDefinition(int *i, int *j, const char *aMove, char *direction);

#endif
