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
const char VALID_MOVES[] = {'N', 'S', 'W', 'E','\0'};

//first, second, height, width
bool are_identical(char** first, char** second, int height, int width);

//make move(board, move, height, width)
bool make_move(char** board, const char* move, int height, int width);

// Move sequence: D3E,D1S,C3E,F3W
bool find_solution(char** begin, char** end, int height, int width, char* solution, int move_num = 0, int skip_num = 0);

#endif