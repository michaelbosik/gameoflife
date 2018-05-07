/* Game of Life
*/

#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"

//declare method prototypes
int checkAnyAlive(char**, int, int);
int compareGrid(char**, char**, int, int);
int fileReader(FILE*, char**, int, int);
void printArray(char**, int, int);
int checkSurroundings(char**, int, int, int, int);
int PlayOne(char**, char**, int, int);

/** Main function.
 * @param argc Number of words on the command line.
 * @param argv Array of pointers to character strings containing the
 *    words on the command line.
 * @return 0 if success, 1 if invalid command line or unable to open file.
 *
 *@author Michael Bosik
 */
int main(int argc, char **argv) {
	printf("Game of Life\n");

	char *inputFileName; // Name of file containing initial grid
	FILE *input; // Stream descriptor for file containing initial grid
	int rows; // Number of rows in the grid
	int columns; // Number of columns in the grid
	int gens; // Number of generations to produce
	int genCount; // Keeps track of generations processed
	int doPrint; // 1 if user wants to print each generation, 0 if not
	int doPause; // 1 if user wants to pause after each generation, 0 if not
	char **pastGrid; // A 2D array to hold the previous pattern
	char **presentGrid; // A 2D array to hold the present pattern
	char **futureGrid; // A 2D array to hold the next pattern
	int go; // True if the main loop should continue looping

	// See if there are the right number of arguments on the command line
	if ((argc < 5) || (argc > 7)) {
		// If not, tell the user what to enter.
		printf("Usage:\n");
		printf("  ./life rows columns generations inputFile [print] [pause]\n");
		return EXIT_FAILURE;
	}

	/* Save the command-line arguments.
	   Also need to check if print and/or pause arguments were entered,
	   and if so, what they were.
	*/
	rows = atoi(argv[1]); // Convert from character string to integer.
	columns = atoi(argv[2]);
	gens = atoi(argv[3]);
	inputFileName = argv[4];

	//set the generation count to start at 0;
	genCount = 0;

	//set the go boolean to true so that the main loop will iterate until false
	go = 1;

	//changes doPrint and doPause based on the 5th and 6th arguments that the user provides: 1 if 'y', 0 if 'n' or null
 	if (argc>5 && *argv[5] == 'y')
 		doPrint = 1;
 	else
 		doPrint = 0;

 	if (argc>6 && *argv[6] == 'y')
 		doPause = 1;
 	else
 		doPause = 0;

 	//check if the user entered negative rows or columns
 	if(rows<1||columns<1){
 		printf("ERROR: rows and columns must be greater than 0.");
 		return EXIT_FAILURE;
 	}

 	//allocate memory for 3 grids: past, present and future
	pastGrid = make2Dchar(rows, columns);
	presentGrid = make2Dchar(rows, columns);
	futureGrid = make2Dchar(rows, columns);

	//open the input file for reading
	input = fopen(inputFileName, "r");

	//if the file is not found, output an error
	if (!input) {
		printf("Unable to open input file: %s\n", inputFileName);
		return EXIT_FAILURE;
	}

	//can't pause between generations if they aren't printing
	if(!doPrint&&doPause){
		printf("ERROR: Cannot pause with printing off.\n");
		return EXIT_FAILURE;
	}

	//calls the fileReader method with the input file, the presentGrid, the rows and columns
	fileReader(input, presentGrid, rows, columns);

	//always print the first generation even if the user opts for no generation printing
	printArray(presentGrid, rows, columns);

	//main loop will iterate until either error condition is met or the generation count exceeds the user's given amount of generations
	while(go && genCount < gens){

		//make a fresh futureGrid through every iteration
		futureGrid = make2Dchar(rows, columns);

		//writes in the futureGrid which cells should become/stay alive and which ones should die
		PlayOne(presentGrid, futureGrid, rows, columns);

		//changes go to true or false if any of the error conditions return false
		go = (checkAnyAlive(presentGrid, rows, columns) && compareGrid(presentGrid, futureGrid, rows, columns) && compareGrid(pastGrid, futureGrid, rows, columns));

		//if the user has opted to pause on each generation, prompts the user to press enter in order to proceed.
		if(doPause&&go){
			char c;//blank character to use in the scanf statement
			printf("\nPress 'Enter' for next generation. There are %d generations left.\n", gens-genCount);
			scanf("%c", &c);
		}

		//changes the 3 arrays so that the future becomes the present and the present becomes the past
		pastGrid = presentGrid;
		presentGrid = futureGrid;

		//only print each generation if the go variable is true and the user has opted for generation printing
		if(go){
			if(doPrint)
				printArray(presentGrid, rows, columns); //prints the current generation
			genCount++; //increased the generation count so that the while loop may end appropriately when the generation count exceeds the user's given generations
		}
	}

	//print the last generation even if the user has opted for no generation printing
	if(!doPrint)
		printArray(presentGrid, rows, columns);

	//print the amount of generations that were processed before termination
	printf("%d generations have been processed. Exiting...\n", genCount);

	return EXIT_SUCCESS;
}

/** Checks if all of the cells in the generation are dead or not
 * @param presentGrid the grid containing the current generation
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return 1 if there are still alive cells in the given array, 0 if there are no more alive cells in the given array
 *
 * @author Michael Bosik
 */
int checkAnyAlive(char** presentGrid, int rows, int columns){

	//iterate through the rows and columns of the user's input in order to check every cell in arrays
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			if(presentGrid[i][j] == 'x') //check if the cell at the current row and column is alive
				return 1; //return true if any alive cells
	printf("ERROR: No alive cells.\n");
	return 0; //return false if no alive cells
}

/** Compares two given arrays to see if they are the same
 * @param gridA the first grid to compare with the second grid
 * @param gridB the second grid to compare with the first grid
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return 1 if the two grids are not the same, 0 if the two grids are exactly the same
 *
 * @author Michael Bosik
 */
int compareGrid(char** gridA, char** gridB, int rows, int columns){

	//iterate through the rows and columns of the user's input in order to check every cell in arrays
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			if(gridA[i][j] != gridB[i][j]) //check if the cell at the current row and column in both arrays are different
				return 1; //return true if any cells are different
	printf("ERROR: Duplicate or repeating generations.\n");
	return 0; //return false if no cells are different
}

/** Creates array using data read from an input file and centers it on a grid
 * @param input the input file containing the pattern that represents the starting generation
 * @param array the array of characters that will be filled with the contents of the input file
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return 0 if completed successfully
 *
 * @author Michael Bosik
 */
int fileReader(FILE* input, char** array, int rows, int columns){

	int numRows = 0; //numRows is used to count the amount of rows that contain alive cells in the given input file
	int numColumns = 0; //numColumns is used to determine the maximum amount of columns out of the rows that contain alive cells in the given input file

	char **tempArray = make2Dchar(rows, columns); //tempArray is used to hold the contents of the input file to then be inserted in the middle of array

	//iterate through the rows of tempArray
	for(int i=0; i<rows; i++)
		fgets(tempArray[i], columns, input); //read each line of the input file for the amount of columns entered by the user and input each cell into tempArray

	//iterate through the rows and columns that the user has specified
	for(int i=0; i<rows; i++){
		int columnAmt = 0; //columnAmt keeps track of the amount of columns in each row to help figure out which row has the most columns
		for(int j=0; j<columns; j++){

			//check if the cell at the current row and column contains an 'o' or an 'x'
			if(array[i][j]=='o'||array[i][j]=='x'){
				columnAmt++; //increased the column amount to keep track of how many columns in the current row contain an 'o' or 'x'
				if(j==0) //only execute the next line one time for each row
					numRows++; //increase the amount of rows that contain alive cells by one
			}
			array[i][j] = ' '; //simultaneously fill the array with blank space
		}
		if(columnAmt>numColumns) //check if the amount of columns in the current row exceeds the current maximum number of columns
			numColumns = columnAmt; //set the maximum number of columns to the amount of columns in the current row
	}

	int startingRow = ((rows-numRows)/2); //determine the starting row to start inserting the contents of the input file in the middle of the array grid
	int startingCol = ((columns-numColumns)/2); //determine the starting columns to start inserting the contents of the input file in the middle of the array grid

	//iterate through the rows and columns of the user's input starting at the determined startingRow and startingCol in order to print in the middle of the given grid
	for(int i=startingRow; i<rows; i++)
		for(int j=startingCol; j<columns; j++)
			array[i][j] = tempArray[i-startingRow][j-startingCol]; //set the cell at the current row and column in array to the corresponding row and column offset by the startingRow and startingCol in tempArray. This fills in the information to the middle of array.
	return 0; //return 0 if successful
}

/** Prints the given array to the console.
 * @param array the array in which to print to the console
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return nothing
 *
 * @author Michael Bosik
 */
void printArray(char** array, int rows, int columns){

	//iterate through the rows and columns of the user's input in order to check every cell in arrays
	for(int i=0; i<rows; i++){
		for(int j=0; j<columns; j++)
			if(array[i][j]=='x') //check if the cell at the current row and column is alive
				printf("%c", array[i][j]); //print to the console the contents of the cell if it is alive
			else
				printf(" "); //print a space for all dead cells
		printf("\n"); //new line after every row
	}
}

/** Checks the adjacent cells to the cell at the given row and column to see how many are alive
 * @param array the array to check the surroundings of the given cell with
 * @param row the specific row in the array that the cell is located to have the surroundings checked
 * @param columns the specific column in the array that the cell is located to have the surroundings checked
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return 1 if the cell at the given row and column should be changed to an alive cell or remain an alive cell, 0 if the cell at the given row and column should be changed to a dead cell
 *
 * @author Michael Bosik
 */
int checkSurroundings(char** array, int row, int column, int rows, int columns){
	int amountAlive = 0; //amountAlive counts the amount of cells that are surrounding the cell in the given row and column that are alive

		//iterate through the cells in the rows and columns in a 3x3 area around the given row and column
		for(int i=row-1;i<=row+1;i++)
			for(int j=column-1;j<=column+1;j++)

				//exclude the cells above the top row, below the bottom row, to the left of the left column and to the right of the right column
				if(i>=0 && j>=0 && i<rows && j<columns)

					//check if the row and column is the same: meaning it the center of the 3x3 area
					if(!(i==row&&j==column))
						if(array[i][j]=='x') //check if the cell is alive
							amountAlive++; //increase the amount of cells that are alive around the cell that is being checked

	//check if the amount of alive cells around the cell in the given row and column is 2 or three and make it either come to life or stay alive if already alive
	if(amountAlive == 3 || (amountAlive == 2 && array[row][column] == 'x'))
		return 1; //return 1 if the cell should be alive
	else
		return 0; //return 0 if the cell should be dead
}

/** Creates the next generation as an array
 * @param currentGen the array that represents the current generation to be examined
 * @param nextGen the array that represents the future generation that is to be created based on the current generation
 * @param rows the amount of rows in the grid that the user has entered
 * @param columns the amount of columns in the grid that the user has entered
 *
 * @return 0 if completed successfully
 *
 * @author Michael Bosik
 */
int PlayOne(char** currentGen, char** nextGen, int rows, int columns){

	//iterate through the amount of rows and columns that the user has entered
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)

			//check if the returned value from calling checkSurroundings is true or false;
			if (checkSurroundings(currentGen, i, j, rows, columns))
				nextGen[i][j] = 'x'; //set the cell in the nextGen grid to be alive if the value is true
			else
				nextGen[i][j] = ' '; //set the cell in the nextGen grid to be dead if the value is false

	return 0; //return 0 if completed successfully
}
