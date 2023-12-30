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

// q1 : get position(board, ch, row, column)
bool get_position(char** board, const char ch, int& row, int& column);

// q2 : valid or invalid solution

bool valid_solution(char** board, char** words);

// q3 : update(board, 'X', x); update(board, ch, mask)

void update(char** board, const char ch, Mask& mask);

// q3b: neighbourhood intersect(one, two)
void neighbourhood_intersect(Mask& one, Mask& two);

// q4 recursive and should use masks
bool solve_board(char** board, char** words);