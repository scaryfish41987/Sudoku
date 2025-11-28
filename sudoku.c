#include "sudoku.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

Step stack[ 100 ];
int top = 0;

void pushStep ( int r, int c, int v ) {
	stack[ top ].row = r;
	stack[ top ].col = c;
	stack[ top ].val = v;
	top++;
}

int **createBoard ( void ) {
	int **board = ( int ** )calloc ( large, sizeof ( int * ) );

	for ( int i = 0; i < large; i++ ) {
		board[ i ] = ( int * )calloc ( large, sizeof ( int ) );
	}

	return board;
}

int **generateBoard ( void ) {
	srand ( time ( NULL ) );
	int **board = createBoard();

	int strategy = rand() % 10;

	int nums = rand() % 25 + 17;

	if ( strategy < 7 ) {
		int **tempBoard = createBoard();
		if ( createSudoku ( tempBoard ) ) {
			int placed = 0;

			while ( placed <= nums ) {
				int row = rand() % large;
				int col = rand() % large;

				if ( tempBoard[ row ][ col ] != 0 ) {
					board[ row ][ col ] = tempBoard[ row ][ col ];
					placed++;
				}
			}
		}

		for ( int i = 0; i < large; i++ ) {
			free ( tempBoard[ i ] );
		}
		free ( tempBoard );
	} else {
		for ( int i = 0; i < nums; i++ ) {
			int attempts = 0;
			int maxAttempts = 20;
			bool placed = false;

			while ( ! placed && attempts < maxAttempts ) {
				int row = rand() % large;
				int col = rand() % large;
				int num = rand() % 9 + 1;

				if ( board[ row ][ col ] == 0 && validPosition ( board, num, row, col ) ) {
					board[ row ][ col ] = num;
					placed = true;
				}
				attempts++;
			}
		}
	}

	return board;
}

bool validPosition ( int **board, int num, int row, int col ) {
	if ( ( num < 0 || num > 9 ) || ( row < 0 || row >= 9 ) || ( col < 0 || col >= 9 ) ) return false;
	for ( int j = 0; j < large; j++ )
		if ( num == board[ row ][ j ] ) return false;

	for ( int i = 0; i < large; i++ )
		if ( num == board[ i ][ col ] ) return false;

	int rstart = ( row / 3 ) * 3, cstart = ( col / 3 ) * 3;

	for ( int i = rstart; i < rstart + 3; i++ )
		for ( int j = cstart; j < cstart + 3; j++ )
			if ( num == board[ i ][ j ] ) return false;

	return true;
}

int *getCandidates ( int **board, int row, int col ) {
	int *used = ( int * )calloc ( 10, sizeof ( int ) );

	for ( int j = 0; j < 9; j++ ) {
		if ( board[ row ][ j ] > 0 ) {
			used[ board[ row ][ j ] ] = 1;
		}
	}

	for ( int i = 0; i < 9; i++ ) {
		if ( board[ i ][ col ] > 0 ) {
			used[ board[ i ][ col ] ] = 1;
		}
	}

	int rowStart = ( row / 3 ) * 3;
	int colStart = ( col / 3 ) * 3;

	for ( int i = rowStart; i < rowStart + 3; i++ ) {
		for ( int j = colStart; j < colStart + 3; j++ ) {
			if ( board[ i ][ j ] > 0 ) {
				used[ board[ i ][ j ] ] = 1;
			}
		}
	}

	return used;
}

bool createSudoku ( int **board ) {
	int lowerCol = -1, lowerRow = -1;
	int min = 999;

	for ( int i = 0; i < large; i++ ) {
		for ( int j = 0; j < large; j++ ) {
			if ( board[ i ][ j ] > 0 ) continue;
			int *used = getCandidates ( board, i, j );
			int m = 0;
			for ( int k = 1; k <= 9; k++ )
				if ( used[ k ] == 0 ) m++;
			if ( m < min ) {
				min = m;
				lowerRow = i;
				lowerCol = j;
			}
			free ( used );
		}
	}

	if ( lowerRow == -1 && lowerCol == -1 ) return true;

	int *used = getCandidates ( board, lowerRow, lowerCol );
	for ( int k = 1; k <= 9; k++ ) {
		if ( used[ k ] == 0 && validPosition ( board, k, lowerRow, lowerCol ) ) {
			board[ lowerRow ][ lowerCol ] = k;
			if ( createSudoku ( board ) ) {
				free ( used );
				return true;
			}
			board[ lowerRow ][ lowerCol ] = 0;
		}
	}
	free ( used );

	return false;
}

bool solveSudoku ( int **board, bool backtracking ) {
	int lowerCol = -1, lowerRow = -1;
	int min = 999;

	for ( int i = 0; i < large; i++ ) {
		for ( int j = 0; j < large; j++ ) {
			if ( board[ i ][ j ] > 0 ) continue;
			int *used = getCandidates ( board, i, j );
			int m = 0;
			for ( int k = 1; k <= 9; k++ )
				if ( used[ k ] == 0 ) m++;
			if ( m < min ) {
				min = m;
				lowerRow = i;
				lowerCol = j;
			}
			free ( used );
		}
	}

	if ( lowerRow == -1 && lowerCol == -1 ) return true;

	int *used = getCandidates ( board, lowerRow, lowerCol );
	for ( int k = 1; k <= 9; k++ ) {
		if ( used[ k ] == 0 && validPosition ( board, k, lowerRow, lowerCol ) ) {
			board[ lowerRow ][ lowerCol ] = k;
			if ( backtracking ) printBoardColored ( board, lowerRow, lowerCol, true );
			if ( solveSudoku ( board, backtracking ) ) {
				pushStep ( lowerRow, lowerCol, k );
				free ( used );
				return true;
			}
			if ( backtracking ) printBoardColored ( board, lowerRow, lowerCol, false );
			board[ lowerRow ][ lowerCol ] = 0;
		}
	}
	free ( used );

	return false;
}

void PrintSolution ( int **board ) {
	for ( int i = 0; i < top; i++ ) {
		board[ stack[ i ].row ][ stack[ i ].col ] = 0;
	}

	for ( int i = top - 1; i >= 0; i-- ) {
		int r = stack[ i ].row;
		int c = stack[ i ].col;
		int v = stack[ i ].val;

		board[ r ][ c ] = v;
		printBoardColored ( board, r, c, true );
	}
}

void printBoard ( int **board ) {
	printf ( "┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓\n" );
	for ( int i = 0; i < large; i++ ) {
		printf ( "┃" );
		for ( int j = 0; j < large; j++ ) {
			printf ( " " );
			if ( board[ i ][ j ] == 0 )
				printf ( " " );
			else
				printf ( "%d", board[ i ][ j ] );

			if ( j == 2 || j == 5 )
				printf ( " ┃" );
			else if ( j == 8 )
				printf ( " ┃" );
			else
				printf ( " │" );
		}

		printf ( "\n" );
		if ( i == 2 || i == 5 )
			printf ( "┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫\n" );
		else if ( i != 8 )
			printf ( "┠───┼───┼───╂───┼───┼───╂───┼───┼───┨\n" );
	}

	printf ( "┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛\n" );
	printf ( "\n" );
}

void printBoardColored ( int **board, int highlightRow, int highlightCol, bool isSuccess ) {
	const char *green = "\033[32m";
	const char *red = "\033[31m";
	const char *reset = "\033[0m";

	printf ( "┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓\n" );

	for ( int i = 0; i < large; i++ ) {
		printf ( "┃" );
		for ( int j = 0; j < large; j++ ) {
			printf ( " " );
			if ( board[ i ][ j ] == 0 )
				printf ( " " );
			else if ( i == highlightRow && j == highlightCol ) {
				if ( isSuccess )
					printf ( "%s%d%s", green, board[ i ][ j ], reset );
				else
					printf ( "%s%d%s", red, board[ i ][ j ], reset );
			} else
				printf ( "%d", board[ i ][ j ] );

			if ( j == 2 || j == 5 )
				printf ( " ┃" );
			else if ( j == 8 )
				printf ( " ┃" );
			else
				printf ( " │" );
		}
		printf ( "\n" );

		if ( i == 2 || i == 5 )
			printf ( "┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫\n" );
		else if ( i != 8 )
			printf ( "┠───┼───┼───╂───┼───┼───╂───┼───┼───┨\n" );
	}

	printf ( "┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛\n" );
	printf ( "\n" );
	usleep ( 300000 );
	system ( "clear" );
}
