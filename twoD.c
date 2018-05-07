#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"

/** Make a 2D array of integers
 *
 * @param rows Number of rows
 * @param columns Number of columns
 * @return Pointer to the array of pointers to the rows.
 * 	  or null pointer if unable to allocate memory.
 * 	  Note: Will not free partially-allocated memory.
 *
 * @author Mike Ciaraldi
 */

int** make2Dint(int rows, int columns) {

	int **a; // Array of pointers to rows
	unsigned int i; // Loop counter

	// First allocate the array of pointers to rows
	a = (int **) malloc(rows * sizeof(int *));
	if (!a) { // Unable to allocate the array
		return (int **) NULL;
	}

	// Now allocate array for each row
	for (i = 0; i < rows; i++) {
		// i is the row we are about to allocate
		a[i] = malloc(columns * sizeof (int));
		if (!a[i]) {
			return (int **) NULL; // Unable to allocate
		}
	}
	return a;
}

/** Make a 2D array of characters
 * @param rows Number of rows
 * @param columns Number of columns
 *
 * @return pointer to the array of pointers to the rows
 * or null pointer if unable to allocate memory
 *
 * @author Michael Bosik
 */
char** make2Dchar(int rows, int columns) {

	char **a; //character array a is used to allocate memory for the amount of rows and columns given
	a = (char **) malloc(rows * sizeof(char *)); //allocate memory for the rows

	if (!a) //Unable to allocate the array
		return (char **) NULL;

	//iterate through the rows of the array
	for(int i=0; i<rows; i++){
		a[i] = malloc(columns * sizeof(char)); //allocate memory for the columns in each row
		if(!a[i]) //Unable to allocate the array
			return (char **) NULL;
	}

	return a;
}
