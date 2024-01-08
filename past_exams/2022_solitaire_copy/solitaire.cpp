#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>

#include "solitaire.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a board */
bool get_board_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  cout << "Loading puzzle board from '" << filename << "'...";
  
  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  cout << " done (height = " << height << ", width = " << width << ")." << endl;
    
  return m;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << static_cast<char>('A' + c);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your function definitions below */

/*Q1
Write a Boolean function find solution(begin, end, height, width, solution) which
attempts to generate a sequence of no more than six valid moves which will transform
board begin into board end (both boards having dimensions height × width). If a sequence
of valid moves can be found, the function should return true and output parameter solution
should contain a comma-separated list of moves. Otherwise the function should return false.
*/

bool are_identical(char** board1, char** board2, int height, int width){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (board1[r][c]!=board2[r][c]){
        return false;
      }
    }
  }
  return true;
}

/*Q2
Write a Boolean function make move(board, move, height, width) which attempts to
make a move described by a 3-character string move on the height × width puzzle board.
* @param: The first character of the move is a letter describing the column of the source marble (‘A’–
‘G’). 
* @param: The second character is a digit describing the row of the source marble (‘0’–’6’). 
* @param: The third character is a letter describing the direction in which the move should be attempted:
‘N’, ‘S’, ‘E’ or ‘W’ for north, south, east and west, respectively.
*/

bool parse_direction(const char direction, int& row, int& col){
  row = 0; col = 0;
  switch(direction){
    case 'N': row--; break;
    case 'S': row++; break;
    case 'W': col--; break;
    case 'E': col++; break;
    default: return false;
  }
  return true;
}

bool parse_move(const char* move, int& row, int& col, int& row_dir, int& col_dir, int height, int width){
  col = move[0] - 'A';
  row = move[1] - '0';
  char direction = move[2];

  // checks if the provided input is valid
  if (row < 0 || row >= height || col < 0 || col >= width){
    return false;
  }

  if (!parse_direction(direction, row_dir, col_dir)){
    return false;
  }

  return true;
}

// make_move(puzzle, "D1S", height, width)
bool make_move(char** board, const char* move, int height, int width){
  int row_current, col_current, row_dir, col_dir; 

  // parse move to extract row, col and direction
  if (!parse_move(move, row_current, col_current, row_dir, col_dir, height, width)){
    return false;
  }
  // CHECK THAT THE MOVE IS VALID
  // check that there is a stone in the current cell, letter O
  if (board[row_current][col_current]!='O'){
    return false;
  }

  int row_dest = row_current + 2*row_dir;
  int col_dest = col_current + 2*col_dir;

  // check the destination is in bounds
  if (row_dest < 0 || row_dest >= height || col_dest < 0 || col_dest >= width){
    return false;
  }

  // check that is an empty cell
  if (board[row_dest][col_dest]!='_'){
    return false;
  }

  // check that there is a stone in the cell in-between
  if (board[row_current + row_dir][col_current + col_dir]!='O'){
    return false;
  }
  // all checks passed, start updating the board

  // remove the stone from the current position
  board[row_current][col_current]='_';
  // remove from the mid position
  board[row_current + row_dir][col_current + col_dir]='_';
  // add to the destination
  board[row_dest][col_dest]='O';
  
  return true;
}

/*Q3
Write a Boolean function find solution(begin, end, height, width, solution) which
attempts to generate a sequence of no more than six valid moves which will transform
board begin into board end (both boards having dimensions height × width). If a sequence
of valid moves can be found, the function should return true and output parameter solution
should contain a comma-separated list of moves. Otherwise the function should return false.
*/

// bool result = find_solution(first, second, h, w, sequence);

const char MOVES[] = "NSWE";

void form_move(char* move, int row, int col, const char m){
  // column
  move[0] = static_cast<char>(col + 'A'); 
  // row
  move[1] = static_cast<char>(row + '0');
  // direction
  move[2] = m;
  move[3] = '\0';
}

void undo_move(char** board, const char* move, int height, int width){
  int row_source, col_source, row_dir, col_dir; 

  // parse move to extract row, col and direction
  parse_move(move, row_source, col_source, row_dir, col_dir, height, width);

  // put the stone back
  board[row_source][col_source] = 'O';
  // put the stone in-between back
  board[row_source + row_dir][col_source + col_dir] = 'O';
  // remove the stone from the destination
  board[row_source + 2*row_dir][col_source + 2*col_dir] = '_';
}

void remove_last_move(char* solution){
  // find the last comma occurence
  int len = strlen(solution);
  // place the terminating character there
  solution[len - 4] = '\0';
}

void add_move(const char* move, char* solution){
  strcat(solution, move);
  strcat(solution, ",");
}

/* simpler but longer solution */
bool find_solution(char** begin, char** end, int height, int width, char* solution, int depth){
  char move[4];

  // extra precaution
  if (depth == 0){
    solution[0]='\0';
  }
  // success - terminate
  if (are_identical(begin, end, height, width)){
    // remove the last comma
    solution[strlen(solution)-1]='\0';
    return true;
  }
  // fail - terminate
  if (depth > 6){
    return false;
  }
  
  // find the first possible move
  for (int row = 0; row < height; row ++){
    for (int col = 0; col < width; col ++){
      // if the cell is not O skip
      if (begin[row][col] != 'O'){
        continue;
      }

      // if it is a stone, we attempt to form a move
      // loop through all the possible directions
      for (const char* m = MOVES; *m; m++){
        form_move(move, row, col, *m);
        if (make_move(begin, move, height, width)){
          // add the move to the solution sequence
          add_move(move, solution);

          if (find_solution(begin, end, height, width, solution, depth + 1)){
            return true;
          }
          else{
            // undo the move on the board
            undo_move(begin, move, height, width);
            // remove the move from the solution sequence
            remove_last_move(solution);
          }
        }
      }
    }
  }

  return false;
}

/*
bool find_next_stone(char** board, int& row, int& col, int h, int w, int skip){
  int skip_counter = 0;
  for (int r = 0; r < h; r++){
    for (int c = 0; c < w; c++){
      if (board[r][c] == 'O'){
        if (skip_counter >= skip){
            row = r;
            col = c;
            return true;
        }
        else{
          skip_counter++;
        }
      }
    }
  }
  return false;
}

bool find_solution(char** begin, char** end, int height, int width, char* solution, int depth, int skip){
  char move[4];

  // extra precaution
  if (depth == 0){
    solution[0]='\0';
  }
  // success - terminate
  if (are_identical(begin, end, height, width)){
    // remove the last comma
    solution[strlen(solution)-1]='\0';
    return true;
  }
  // fail - terminate
  if (depth > 6){
    return false;
  }
  // find the next stone skipping the number of invalid
  int row, col;
  if (!find_next_stone(begin, row, col, height, width, skip)){
    // something went wrong, no stone
    return false;
  }

  // if it is a stone, we attempt to form a move
  // loop through all the possible directions
  for (const char* m = MOVES; *m; m++){
    form_move(move, row, col, *m);
    if (make_move(begin, move, height, width)){
      // add the move to the solution sequence
      add_move(move, solution);
      if (find_solution(begin, end, height, width, solution, depth + 1, 0)){
        // resetting the skip counter
        return true;
      }
      else{
        // undo the move on the board
        undo_move(begin, move, height, width);
        // remove the move from the solution sequence
        remove_last_move(solution);
      }
    }
  }
  // it is possible that we just need to skip the move
  return find_solution(begin, end, height, width, solution, depth, skip + 1);
}
*/