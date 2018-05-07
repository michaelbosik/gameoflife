Written by Michael Bosik

Files Included:
 - Doxyfile
 - makefile
 - README.txt
 - life.c
 - twoD.c
 - twoD.h
 - Folder (configs):
    - acorn.txt
    - blinker.txt
    - block.txt
    - diehard.txt
    - ends.txt
    - glider.txt
    - r_pentomino.txt
    - spaceship.txt

How to Compile:
 - Navigate to the file directory on your virtual machine's terminal.
 - Enter the command 'make clean && make' to clear any excess memory that may remain in the program and recompile it
 - Enter the command './life [rows] [columns] [generations] [{file directory}] [print generations ('y' or 'n' or blank)] [pause after each generation ('y' or 'n' or blank)]'
    - rows = amount of rows in the desired grid (int)
    - columns = amount of columns in the desired grid (int)
    - generations = amount of generations to be processed (int)
    - {file directory} = location of the input file to be read with the starting configuration
    - print generations = optional input if the user wants the program to print all of the generations or not
    - pause after each generation = optional input if the user wants the program to pause after printing each generation

Possible ERRORS:
 - If the user enters 'n' as the 5th argument (print generations) and 'y' as the 6th argument (pause after each generation):
    - "ERROR: Cannot pause with printing off."
 - If two consecutive generations are exactly the same or two generation configurations alternate repeatedly:
    - "ERROR: Duplicate or repeating generations."
 - If a generation has no alive cells in it:
    - "ERROR: No alive cells."
 - If the user enters an amount of rows or columns less than 1.
    - "ERROR: rows and columns must be greater than 0."
 - If the user enters an invalid amount of arguments (<5 or >7)
    - "Usage:
       ./life rows columns generations inputFile [print] [pause]\n"
 - If the input file cannot be found
    - "Unable to open input file: [filename]"

Test Cases:
 - Command: './life 25 25 50 configs/blinker.txt y y'
    - Output: xxx
              "Error: Duplicate or repeating generations."
              "2 generations have been processed. Exiting..."
 - Command: './life 25 25 25 configs/glider.txt n n'
    - Output: x
               xx
              xx
              "25 generations have been processed. Exiting..."

Difficulties Programming:
 - Figuring out the logistics in order to center the starting array that was included in the input file on the user-entered array.
 - Getting the program to end after alternating and consistent generations were detected.

Loop Invariants:
 - while loop in the main method: the amount of user entered generations that need to be processed.
 - for loops in checkAnyAlive method: the amount of user entered rows and columns in the array
 - for loops in compareGrid method: the amount of user entered rows and columns in the array; the two arrays that are being compared
 - for loops in fileReader method: the amount of user entered rows and columns in the array
 - for loops in printArray method: the amount of user entered rows and columns in the array; the given array that is printed
 - for loops in checkSurroundings method: the amount of user entered rows and columns in the array; the specific row and column of the cell being analyzed; the array being searched
 - for loops in PlayOne method: the amount of user entered rows and columns in the array; the current generation array
