/*
NpuzzleGenerator.cpp
Universidad Simon Bolivar
CI5437: Inteligencia Artificial I

Autores: 
  - Yarima Luciani 13-10770
  - Lautaro Villalon 12-10427

Equipo: LIAY
Prof. Blai Bonet
Ene-Mar 2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

int size, sqSize;

void printRule(int t1, int t2) {

	for (int i=0; i<sqSize; i++) {

		if (i == t1) {
			printf("0 ");
		} else if (i == t2) {
			printf("X ");
		} else {
			printf("- ");
		}
	}

	printf("=> ");

	for (int i=0; i<sqSize; i++) {

		if (i == t2) {
			printf("0 ");
		} else if (i == t1) {
			printf("X ");
		} else {
			printf("- ");
		}
	}
}

int main (int argc, char *argv[]) {

	if (argc != 2) {
		printf("Please provide the height of the puzzle in one integer.\n\n i.e: 15-Puzzle: 4, 24-Puzzle: 5\n\n");
		return -1;
	}

	size = atoi(argv[1]);
	sqSize = size*size;

	printf("DOMAIN tile %d\n\t", sqSize);

	for (int i=0; i<sqSize; i++) {
		printf("%d ", i);
	}

	printf("\n\n%d\n\n", sqSize);

	for (int i=0; i<sqSize; i++) {
		printf("tile ");
	}

	printf("\n\n");

	for (int i=0; i<sqSize; i++) {
		
		// Blank moves up
		if (i >= size) {
			printRule(i, i-size);
			printf("LABEL move_blank_up_%d\n", i);

		}

		// Blank moves down
		if (i < (size-1)*size) {
			printRule(i, i+size);
			printf("LABEL move_blank_down_%d\n", i);
		} 

		// Blank moves left
		if (i%size != 0) {
			printRule(i, i-1);
			printf("LABEL move_blank_left_%d\n", i);
		}

		//Blank moves right
		if (i%size != (size-1)) {
			printRule(i, i+1);
			printf("LABEL move_blank_right_%d\n", i);
		}

	}

	printf("\nGOAL ");
	for (int i=0; i<sqSize; i++) {
		printf("%d ", i);
	}

	printf("\n");
	return 1;
}