#pragma once

#include <stdbool.h>

#define large 9

typedef struct {
	int row, col, val;
} Step;

void pushStep ( int r, int c, int v );

int **createBoard ( void );

int **generateBoard ( void );

bool validPosition ( int **, int, int, int );

int *getCandidates ( int **, int, int );

bool createSudoku ( int ** );

bool solveSudoku ( int **, bool );

void PrintSolution ( int ** );

void printBoard ( int ** );

void printBoardColored ( int **, int, int, bool );
