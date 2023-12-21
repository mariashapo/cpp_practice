#ifndef MAZE_H
#define MAZE_H
/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

/* a function to find a marker */
bool find_marker(char marker, char** maze, int height, int width, int& row, int& column);

bool valid_solution(const char* path, char** maze, int height, int width, char start_marker = '>', char end_marker = 'X');

bool moveMaze(const char direction, int height, int width, int& row, int& col);

const char* find_path(char** maze, int height, int width, char start, char end);

bool find_solution(char** maze, char* str, int height, int width, int current_row, int current_col, char end_marker);

#endif