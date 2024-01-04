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

/* my functions */

const char DIRECTIONS[] = {'N', 'E', 'S', 'W'};

/*Write a function find marker(ch, maze, height, width, row,
column) which finds the coordinates of marker character ch in the
2D array of characters maze of dimension height × width. row
and column are output parameters. When the maze contains the
marker character, row and column should be set to the row and
column coordinates of the marker character respectively, and the
function should return true. If the maze does not contain the
marker character row and column should both be set to -1, and
the function should return false.*/
bool find_marker(const char ch, char** maze, int height, int width, int& row, int& col);

/*
Write a function valid solution(path, maze, height, width)
which determines 
- if a given path through a height × width maze
leads from the entrance marker ’>’ to the exit marker ’X’ 
- without moving outside the boundaries of the maze 
- or passing through a hedge. 
*/
bool valid_solution(const char* path, char** maze, int height, int width, const char start_char = '>', const char end_char = 'X');

/*
[ ] Write a function find path(maze, height, width, start, end)
which finds a valid solution path through a height × width maze
beginning at marker character start and terminating at marker
character end. 
[ ] The path should be marked on the maze using ’#’
characters, and the function should return a string of direction
movements. 
[ ] If there is no path, the function should return the
string "no solution".
*/
const char* find_path(char** maze, int height, int width, const char start_marker, const char end_marker);
#endif