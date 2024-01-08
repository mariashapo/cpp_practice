#ifndef SOLITAIRE_H
#define SOLITAIRE_H
/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width);

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width);

/* place your headers below */

/*Q1
Write a Boolean function find solution(begin, end, height, width, solution) which
attempts to generate a sequence of no more than six valid moves which will transform
board begin into board end (both boards having dimensions height × width). If a sequence
of valid moves can be found, the function should return true and output parameter solution
should contain a comma-separated list of moves. Otherwise the function should return false.
*/

bool are_identical(char** board1, char** board2, int height, int width);

/*Q2
Write a Boolean function make move(board, move, height, width) which attempts to
make a move described by a 3-character string move on the height × width puzzle board.
* @param: The first character of the move is a letter describing the column of the source marble (‘A’–
‘G’). 
* @param: The second character is a digit describing the row of the source marble (‘0’–’6’). 
* @param: The third character is a letter describing the direction in which the move should be attempted:
‘N’, ‘S’, ‘E’ or ‘W’ for north, south, east and west, respectively.
*/

bool make_move(char** board, const char* move, int height, int width);

/*Q3
Write a Boolean function find solution(begin, end, height, width, solution) which
attempts to generate a sequence of no more than six valid moves which will transform
board begin into board end (both boards having dimensions height × width). If a sequence
of valid moves can be found, the function should return true and output parameter solution
should contain a comma-separated list of moves. Otherwise the function should return false.
*/
bool find_solution(char** begin, char** end, int height, int width, char* solution, int depth = 0);
//bool find_solution(char** begin, char** end, int height, int width, char* solution, int depth = 0, int skip = 0);

#endif