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

bool are_identical(char** first, char** second, int height, int width){
  for (int r = 0; r<height; r++){
    for (int c =0; c< width; c++){
      if (first[r][c]!=second[r][c]){
        return false;
      }
    }
  }
  return true;
}

bool in_bounds(int height, int width, int row, int col){
  if (row>=height || row<0 || col >= width || col<0){
    //cerr << "out of bound" << endl;
    return false;
  }
  if ((col < 2 || col > 4) && (row <2 || row >4)){
    return false;
  }
  else {return true;}
}

bool convert_position(const char* move, int height, int width, int& row, int& col){
  col = move[0] - 'A';
  row = move[1] - '0';
  if (!in_bounds(height, width, row, col)){
    return false;
  }
  return true;
}

void convert_position(const char* move, int& row, int& col){
  col = move[0] - 'A';
  row = move[1] - '0';
}

bool valid_move_notation(const char* move){
  if (move[0] < 'A' || move[0] > 'G'){
    return false;
  }
  if (move[1] < '0' || move[1] > '6'){
    return false;
  }
  if (move[2]!='N' && move[2]!='S' && move[2]!='E' && move[2]!='W'){
    return false;
  }
  return true;
}

bool move_direction(const char move_direction, int& row_direction, int& col_direction){
  switch(move_direction){
    case 'N': row_direction = -1, col_direction = 0; return true;
    case 'S': row_direction = 1, col_direction = 0; return true;
    case 'E': row_direction = 0, col_direction = 1; return true;
    case 'W': row_direction = 0, col_direction = -1; return true;
    case 'X': row_direction = 0, col_direction = 0; return true; // leaving a marble in place?
    default: return false;
  }
}

bool make_move(char** board, const char* move, int height, int width){
  int row, col, row_direction, col_direction;
  if (!valid_move_notation(move)){return false;}

  if (!convert_position(move, height, width, row, col)){
    return false;
  }
  
  // check if there is a marbe at the source
  if (board[row][col]!='O'){
    //cerr << "\nno source marble" << endl;
    return false;
  }
  if (!move_direction(move[2], row_direction, col_direction)){
    //cerr << "\nnot valid range" << endl;
    return false;
  }
  
  if (!in_bounds(height, width, row+2*row_direction, col+2*col_direction)){
    return false;
  }

  if (board[row+row_direction][col+col_direction]!='O'){
    //cerr << "\nno marble in-between" << endl;
    return false;
  }
  // check if the destination is empty
  if (board[row+2*row_direction][col+2*col_direction]!='_'){
    //cerr << "\ndestination not empty" << endl;
    return false;
  }

  // make the move
  board[row][col]='_';
  board[row+row_direction][col+col_direction]='_';
  board[row+2*row_direction][col+2*col_direction]='O';
  
  return true;
}

void index_to_position(char* move, int row, int col){
  move[0]=col+'A';
  move[1]=row+'0';
}

void undo_move(char** board, char* move){
  int row_source, col_source, row_direction, col_direction;
  // recover source
  col_source = move[0] - 'A';

  row_source = move[1] - '0';

  board[row_source][col_source]='O';
  // recover direction
  move_direction(move[2], row_direction, col_direction);

  // recover in-between
  board[row_source+row_direction][col_source+col_direction]='O';

  // recover destination
  board[row_source+2*row_direction][col_source+2*col_direction]='_';

  return;
}

bool find_next_marble(char** board, int height, int width, char* move, int& row, int& col, int skip_num){
  int count = 0;
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (board[r][c]=='O'){
        if (skip_num==count){
          row = r; col = c;
          index_to_position(move, r, c);
          return true;
        }
        count++;
      }
    }
  }
  return false;
}

bool find_solution(char** begin, char** end, int height, int width, char* solution, int move_num, int skip_num){
  char move[4]; int row, col;

  move[3]='\0';
  if (are_identical(begin, end, height, width)){
    solution[((move_num-1)*4)+3]='.';
    return true;
  }

  else if(move_num>=6){
    return false;
  }

  // [X] find the next marble position
  if (!find_next_marble(begin, height, width, move, row, col, skip_num)){
    return false;
  }

  for (int m=0; VALID_MOVES[m]; m++){
    move[2]=VALID_MOVES[m];
    if (!make_move(begin, move, height, width)){
      continue;
    }
    // [X] if move is possible, temporarily accept and move to the next available marbe

    // here we are at a position that a valid move has been found
    if (find_solution(begin, end, height, width, solution, move_num+1)){
      strncpy(solution+(move_num*4), move, 3); // copy in the solution

      if (solution[(move_num*4)+3]!='.'){
        solution[(move_num*4)+3]=',';
      }
      else{
        solution[(move_num*4)+3]='\0';
      }

      return true;
    }
    else{
      undo_move(begin, move);
    }
  }
  
  // [X] Update tested to keep the track of marbles not being moved
  return (find_solution(begin, end, height, width, solution, move_num, skip_num+1));
  
  return false;
}

