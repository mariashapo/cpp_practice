#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */


/*
 * a Boolean function is complete(mines, revealed) 
 * @params: takes two 9 × 9 arrays of characters, the first (mines) representing
mine locations and the second (revealed) the current playing
board
 * @return: returns true if and only if all non-mine squares in the
playing board have been uncovered.
*/

bool is_complete(const char mines[9][9], char revealed[9][9]){
  for (int r = 0; r<9; r++){
    for (int c = 0; c<9; c++){
      if (revealed[r][c]=='?'){
        // still covered
        if (mines[r][c]=='*'){
          // a mine
          return false;
        }
      }
    }
  }
  return true;
}

bool valid_coordinates(int row, int col){
  if (row < 0 || col < 0 || row >=9 || col >=9){
    return false;
  }
  return true;
}

bool set_int_position(const char* position, int& row, int& col){
  row = position[0] - 'A';
  col = position[1] - '1';
  return valid_coordinates(row, col);
}


int count_mines(int row, int col, const char mines[9][9]){
  int count = 0;
  for (int r = row - 1; r<=row+1; r++){
    for (int c = col -1; c<=col+1; c++){
      if (!valid_coordinates(r, c) || (r==row && c==col)){
        continue;
      }
      else if (mines[r][c] == '*') 
      {count ++;}
    }
  }
  return count;
}

int count_symbol(int row, int col, const char mines[9][9], char symbol){
  int count = 0;
  for (int r = row - 1; r<=row+1; r++){
    for (int c = col -1; c<=col+1; c++){
      if (!valid_coordinates(r, c) || (r==row && c==col)){
        continue;
      }
      else if (mines[r][c] == symbol) 
      {count ++;}
    }
  }
  return count;
}

/*
 * a function count mines(position, mines) which returns
the number of mines around a particular square. 
 * @param: position is a two-character string denoting row and column board coor-
dinates (e.g. “I8”)
 * @param: mines is a 2D character array of mine locations.
*/
int count_mines(const char* position, const char mines[9][9]){
  int row, col;
  set_int_position(position, row, col);
  return count_mines(row, col, mines);
}

/*
 * a function make move(position, mines, revealed) which
uncovers or flags a square.
 * @param: mines is an input parameter representing mine locations
 * @param: revealed is an input/output parameter presenting 
 the current playing board. 
 * @param: The first two
characters of position denote the row and column board coordi-
nates. If no other characters are present in position, the move is
to uncover the square in revealed2. If a ’*’ is present as the third
character, the move is to flag the square in revealed. 
 * @return: The return value is of type MoveResult (see minesweeper.h):
enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2,
BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };
 * INVALID MOVE is returned when the row and column coordinates
are not valid or if the third character is present, but is not a ’*’.
 * REDUNDANT MOVE is returned if the target position corresponds to
a square that has already been uncovered or flagged. 
 * BLOWN UP is returned if the move uncovers a mine. 
* SOLVED BOARD is re-
turned if all non-mine squares in revealed have been uncovered.
* Otherwise, VALID MOVE is returned.
*/

MoveResult make_move(const char* position, const char mines[9][9], char revealed[9][9]){
  int row, col;
  bool mine_mark;
  set_int_position(position, row, col);
  if (strlen(position) > 3 || (position[2]!='\0' && position[2]!='*')){
    return INVALID_MOVE;
  }

  mine_mark = (position[2]=='*')?true:false;

  return make_move(row, col, mine_mark, mines, revealed);
}

void reveal_empty(int row, int col, const char mines[9][9], char revealed[9][9]){
  int count; 
  // reveal the empty one
  revealed[row][col] = ' ';

  // reveal all the squares around the empty one
  for (int r = row - 1; r<=row+1; r++){
    for (int c = col -1; c<=col+1; c++){
      if (!valid_coordinates(r, c) || (r==row && c==col)){
        continue;
      }
      else if (revealed[r][c] == '?') {
        count = count_mines(r, c, mines);
        if (count == 0){
          reveal_empty(r, c, mines, revealed);
        }
        else{
          revealed[r][c] = '0' + count;
        }
      }
    }
  }
  return;
}

MoveResult make_move(int row, int col, bool mine_mark, const char mines[9][9], char revealed[9][9]){
  if (!valid_coordinates(row, col)){
    return INVALID_MOVE;
  }

  if (revealed[row][col]!='?'){ // move already made before
    return REDUNDANT_MOVE;
  }

  if (mines[row][col]=='*' && !mine_mark){
    return BLOWN_UP;
  }
  
  if (mine_mark){
    revealed[row][col]='*';
    return VALID_MOVE;
  }

  int num_mines = count_mines(row, col, mines);

  if (num_mines > 0){
    revealed[row][col] = '0' + num_mines;
  }
  else if (num_mines == 0){
    reveal_empty(row, col, mines, revealed);
  }
  
  if (is_complete(mines, revealed)){
    return SOLVED_BOARD;
  }

  return VALID_MOVE;
}

// level - number of cells around required to make a cell safe
bool is_safe(int row, int col, char revealed[9][9]){
  int digit, count_predcited;
  // loop through all the squares around it
  for (int r = row - 1; r<=row+1; r++){
    for (int c = col -1; c<=col+1; c++){
      if (!valid_coordinates(r, c) || (r==row && c==col)){
        continue;
      }
      else if(isdigit(revealed[r][c])){
        digit = revealed[r][c] - '0';
        count_predcited = count_mines(r, c, revealed);
        if (digit == count_predcited){
          return true;
        }
      }
    }
  }
  // no adjacent square that guarantess safety has been found
  return false;
}

// level - number of cells around required to know cell is a mine
bool is_mine(int row, int col, char revealed[9][9]){
  int digit, count_revealed_mines, count_unknown;
  // loop through all the squares around it
  for (int r = row - 1; r<=row+1; r++){
    for (int c = col -1; c<=col+1; c++){
      if (!valid_coordinates(r, c) || (r==row && c==col)){
        continue;
      }
      else if(isdigit(revealed[r][c])){
        digit = revealed[r][c] - '0';
        count_revealed_mines = count_mines(r, c, revealed);
        count_unknown = count_symbol(r, c, revealed, '?');
        if (digit == count_revealed_mines + count_unknown){
          return true;
        }
      }
    }
  }
  // no adjacent square that guarantess safety has been found
  return false;
}

void form_str_position(int row, int col, char* move, bool mine_mark = false){
  move[0] = row + 'A';
  move[1] = col + '1';
  if (!mine_mark){
    move[2] = '\0';
  }
  else{
    move[2]='*';
    move[3]='\0';
  }
}

/*
 * Boolean function find safe move(revealed, move) which
determines if a risk-free move is available starting from the cur-
rent playing board 'revealed'. 
 * The return value of the function should be true if a risk-free 
 move is available, in which case output string move should contain the move. 
 * Otherwise the return value of the function should be false and 
 the output string move should be the empty string.
*/

bool find_safe_move(char revealed[9][9], char* move){
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      if (revealed[row][col]!='?'){
        continue;
      }
      if (is_safe(row, col, revealed)){
        form_str_position(row, col, move);
        return true;
      }
      else if (is_mine(row, col, revealed)){
        form_str_position(row, col, move, true);
        return true;
      }
    }
  }
  return false;
}

/*
* Write a function solve board(mines,revealed,moves) which works
out the set of risky moves4 required to solve a minesweeper puzzle and
outputs a string containing these via output parameter moves. You
can assume safe (risk-free) moves will be made whenever possible (in
between the risky ones). 
* When choosing between competing risky
moves, you should select the one which will (when taken together
with all the safe moves it makes possible) uncover the most squares.
*/

int count_possible_safe(int r, int c, char revealed[9][9]){
  int count = 0;
  // set the move to ' ' temporarily
  revealed[r][c] = ' ';
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      if (revealed[row][col]!='?'){
        continue;
      }
      if (is_safe(row, col, revealed)){
        count++;
      }
      else if (is_mine(row, col, revealed)){
        count++;
      }
    }
  }
  // mask the cell back
  revealed[r][c] = '?';
  return count;
}

void undo_move(const char* move, char revealed[9][9]){
  int row, col;
  set_int_position(move, row, col);
  revealed[row][col] = '?';
}

/*
// method 1 - using a skip counter

bool find_best_risky_move(char revealed[9][9], char* move, int skip){
  int uncovered_safe_moves, max_safe = -1, found = 0;
  bool move_exists = false;
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      // we are only interested in the ones still covered
      if (revealed[row][col]!='?'){
        continue;
      }
      // calculate the number of safe moves the move would enable
      uncovered_safe_moves = count_possible_safe(row, col, revealed);
      if (uncovered_safe_moves > max_safe){
        if (found < skip){
          found++;
          continue;
        }
        max_safe = uncovered_safe_moves;
        form_str_position(row, col, move);
        move_exists = true;
      }
    }
  }
  return move_exists;
}

void solve_board(const char mines[9][9], char revealed[9][9], char* moves, bool first_call){
  char move[512];
  int ret = 0;

  if (first_call){
    moves[0] = '\0';
  }
  
  // make all the safe moves
  while (find_safe_move(revealed, move)) {
    ret = make_move(move, mines, revealed);
    if (ret!=0){
      break;
    }
  }
  // reset the number of attempts
  int attempts = 0;

  // moves rejected
  // while not found a possible risky move

  while (find_best_risky_move(revealed, move, attempts)){
    ret = make_move(move, mines, revealed);
    if (ret<0){
      undo_move(move, revealed);
      attempts++;
    }
    else{
      strcat(moves, move);
      strcat(moves, ", ");
      solve_board(mines, revealed, moves, false);
      break;
    }
  }
}
*/

bool find_best_risky_move(char revealed[9][9], char* move, char* rejected){
  int uncovered_safe_moves, max_safe = -1;
  bool move_exists = false;
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      // we are only interested in the ones still covered
      if (revealed[row][col]!='?'){
        continue;
      }
      // calculate the number of safe moves the move would enable
      uncovered_safe_moves = count_possible_safe(row, col, revealed);
      if (uncovered_safe_moves > max_safe){
        form_str_position(row, col, move);
        if (strstr(rejected,move)!=NULL){
          continue;
        }
        max_safe = uncovered_safe_moves;
        strcat(rejected, move);
        move_exists = true;
      }
    }
  }
  return move_exists;
}


// method 2 keeping track of the rejected moves
void solve_board(const char mines[9][9], char revealed[9][9], char* moves, bool first_call){
  char move[512];
  int ret = 0;

  if (first_call){
    moves[0] = '\0';
  }
  
  // make all the safe moves
  while (find_safe_move(revealed, move)) {
    ret = make_move(move, mines, revealed);
    if (ret!=0){
      break;
    }
  }

  char moves_rejected[512];
  moves_rejected[0] = '\0';

  while (find_best_risky_move(revealed, move, moves_rejected)){
    ret = make_move(move, mines, revealed);
    if (ret<0){
      undo_move(move, revealed);
    }
    else{
      strcat(moves, move);
      strcat(moves, ", ");
      solve_board(mines, revealed, moves, false);
      break;
    }
  }
}