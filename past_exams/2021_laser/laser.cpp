#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "laser.h"

using namespace std;

/* pre-supplied helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **array = new char *[rows];
  assert(array);
  for (int r=0; r<rows; r++) {
    array[r] = new char[columns];
    assert(array[r]);
  }
  return array;
}

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows) {
  for (int r=0; r<rows; r++)
    delete [] array[r];
  delete [] array;
}

/* internal helper function which gets the dimensions of a board */
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

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **board = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    strcpy(board[r], line);
  }
  
  return board;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your own function definitions here */

/*
Write a function find laser(board, height, width, row) which scans the
leftmost column of the height × width 2D array of characters board for the
character ’>’ (representing the laser). Where the leftmost column of board
contains the character, output parameter row should contain the correspond-
ing row index, and the function should return true; otherwise row should be
set to -1 and the function should return false.*/
bool find_laser(char** board, int height, int width, int& row){
  for (int r = 0; r < height; r++){
    if (board[r][0]=='>'){
      row = r;
      return true;
    }
  }
  row = -1;
  return false;
}

/*
Write a function mirror_label(board, height, width, row, column) which
returns the alphabetical character label (if any) attached to the mirror found at
coordinates (row, column) within the height × width 2D array of characters
board. 
If there is no mirror at the coordinates, or if there is no alphabetical
character attached then the function should return the character ’\0’.
*/

char mirror_label(char** board, int height, int width, int row, int column){
  char label;
  if (board[row][column]!='/' && board[row][column]!='\\'){
    return '\0';
  }
  else if (board[row][column]=='\\'){
    label = board[row-1][column-1];
    if (isalpha(label)){
      return label;
    }
    label = board[row+1][column+1];
    if (isalpha(label)){
      return label;
    }
    return '\0';
  }
  else if (board[row][column]=='/'){
    label = board[row-1][column+1];
    if (isalpha(label)){
      return label;
    }

    label = board[row+1][column-1];
    if (isalpha(label)){
      return label;
    }
    return '\0';
  }
  return '\0';
}

/*
Write a Boolean function shoot(board, height, width, message, last row,
last col) which plots the path of the laser beam across the given board,
updating board and output string message appropriately. 
The output parameters last row and last col should be set to the final coordinates of the
laser beam, which should terminate at an edge, the laser light absorber or a
’?’. The function should return true if and only if the final coordinates of
the laser beam correspond to the laser light absorber.
*/

// enum Direction {NORTH, EAST, SOUTH, WEST};
bool set_direction(int& row_dir, int& col_dir, char& symbol, Direction dir){
  switch(dir){
    case NORTH: row_dir = -1, col_dir = 0, symbol = '|'; return true;
    case SOUTH: row_dir = 1, col_dir = 0, symbol = '|'; return true;
    case EAST: row_dir = 0, col_dir = 1, symbol = '-'; return true;
    case WEST: row_dir = 0, col_dir = -1, symbol = '-'; return true;
    default: return false;
  }
}

int make_move(char** board, int& row, int& col, int row_dir, int col_dir, char symbol){
  row += row_dir, col += col_dir;
  char next_cell = board[row][col];
  int result;

  if (next_cell=='@'){
    result = 3;
  }
  else if (next_cell=='#' || next_cell=='?'|| next_cell=='>'){
    result = -1;
  }
  else if (next_cell == ' '){
    board[row][col]=symbol;
    result = 0;
  }
  else if (next_cell == '-' || next_cell == '|' || next_cell == '+'){
    board[row][col]='+';
    result = 0;
  }
  else if (next_cell == '\\' || next_cell == '/'){
    result = 1;
  }
  return result;
}

Direction flip_dir(const char mirror, Direction current_direction){
  if (mirror=='\\'){
    switch (current_direction){
      case SOUTH: return EAST;
      case EAST: return SOUTH;
      case NORTH: return WEST;
      case WEST: return NORTH;
      default: return current_direction;
    }
  }
  else{
    switch (current_direction){
      case NORTH: return EAST;
      case EAST: return NORTH;
      case SOUTH: return WEST;
      case WEST: return SOUTH;
      default: return current_direction;
    }
  }
}

bool shoot(char** board, int height, int width, char* message, int& last_row, int& last_col){
  int row_dir, col_dir, result = 0; // column and row directions
  char symbol;

  // look for '>'
  if (!find_laser(board, height, width, last_row)){
    return false;
  }
  last_col = 0;
  Direction dir = EAST;
  set_direction(row_dir, col_dir, symbol, dir);
  
  while (1){
    result = make_move(board, last_row, last_col, row_dir, col_dir, symbol);

    if (result == 0){
      continue;
    }
    else if (result == 1){ // encountered a mirror
      dir = flip_dir(board[last_row][last_col], dir);
      set_direction(row_dir, col_dir, symbol, dir);
      char letter = mirror_label(board, height, width, last_row, last_col);
      if (letter){
        *message = letter;
        message++;
      }
    }

    else if(result == 3){
      *message = '\0';
      return true;
    }
    else if(result == -1){
      break;
    }
  }
  *message = '\0';
  return false;
}

void clean_board(char** board, int height, int width){
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      if (board[row][col]=='|' || board[row][col]=='-' || board[row][col]=='+'){
        board[row][col]=' ';
      }
    }
  }
}

bool solve(char** board, int height, int width, const char* target){
  char message[512];
  int last_row, last_col;

  // get as far as possible with shoot
  if (shoot(board, height, width, message, last_row, last_col)){
    clean_board(board, height, width);
    // we reached the end, light absorber: @
    if (strcmp(message, target)==0){
      return true;
    }
    else{
      return false;
    }
  }
  
  //print_board(board, height, width);

  if (board[last_row][last_col] != '?'){
    // means bumped into the border
    return false;
  }
  int message_len = strlen(message);
  if (message[message_len - 1]!=target[message_len - 1]){
    return false;
  }

  for (const char* fig = FIGURES; *fig; fig++){
    // for all the possible next moves
    board[last_row][last_col] = *fig;
    clean_board(board, height, width);
    // attempt with the new board
    if (solve(board, height, width, target)){
      return true;
    }
  }
  clean_board(board, height, width);
  return false;
}
