
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sudoku.h"

int main ( void ) {
	int **sudoku = generateBoard();
	bool backtracking = false;
	bool solved;
	char c;

	printf ( "Este es el sudoku a resolver:\n" );
	printBoard ( sudoku );
	printf ( "Quieres visualizar el backtracking (s/n)? " );
	scanf ( " %c", &c );
	if ( c == 's' ) backtracking = true;
	sleep ( 2 );
	system ( "clear" );

	if ( backtracking ) {
		solved = solveSudoku ( sudoku, true );
	} else {
		solved = solveSudoku ( sudoku, false );
		if ( solved ) PrintSolution ( sudoku );
	}
	if ( solved )
		printf ( "El sudoku ha sido solucionado con exito :))))\n" );
	else
		printf ( "El sudoku no tiene solucion :(\n" );

	printBoard ( sudoku );
	return 0;
}
