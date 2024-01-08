enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

// added functions:
bool is_complete(const char mines[9][9], char revealed[9][9]);

/*
 * a function count mines(position, mines) which returns
the number of mines around a particular square. 
 * @param: position is a two-character string denoting row and column board coor-
dinates (e.g. “I8”)
 * @param: mines is a 2D character array of mine locations.
*/
int count_mines(const char* position, const char mines[9][9]);

/* overloaded function */
int count_mines(int row, int col, const char mines[9][9]);

MoveResult make_move(const char* position, const char mines[9][9], char revealed[9][9]);

/*
 * overloaded function make move taking int integer values for the position
 */
MoveResult make_move(int row, int col, bool mine_mark, const char mines[9][9], char revealed[9][9]);


/*
 * Boolean function find safe move(revealed, move) which
determines if a risk-free move is available starting from the cur-
rent playing board 'revealed'. 
 * The return value of the function should be true if a risk-free 
 move is available, in which case output string move should contain the move. 
 * Otherwise the return value of the function should be false and 
 the output string move should be the empty string.
*/
bool find_safe_move(char revealed[9][9], char* move);

// solve_board(mines, revealed, moves);
void solve_board(const char mines[9][9], char revealed[9][9], char* moves, bool first_call = true);