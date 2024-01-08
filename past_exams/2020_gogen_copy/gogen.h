/*  Pre-supplied functions for loading and printing board and word list */

char **load_board(const char *filename);
bool save_board(char **board, const char *filename);
void print_board(char **board);

char **load_words(const char *filename);
void print_words(char **words);

void delete_board(char **board);
void delete_words(char **words);

/* -----------8<-------------8<------------8<--------------8<------------ */

/* add your function prototypes here */

//q1
bool get_position(char** board, const char ch, int& row, int& column);

//q2
bool valid_solution(char** solution, char** words);

//q3
void update(char** board, const char ch, Mask& mask);
void neighbourhood_intersect(Mask& one, Mask& two);

//q4
bool solve_board(char** board, char** words);
bool solve_board_recursive(char** board, char** words);