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

/* Q1
Write a function find laser(board, height, width, row) which scans the
leftmost column of the height × width 2D array of characters board for the
character ’>’ (representing the laser). 
Where the leftmost column of board
contains the character, output parameter row should contain the correspond-
ing row index, and the function should return true; otherwise row should be
set to -1 and the function should return false.*/

bool find_laser(char** board, int height, int width, int& row){
  if (width < 0){
      row = -1;
      return false;
  }
  for (int r = 0; r < height; r++){
    if (board[r][0] == '>'){
      row = r;
      return true;
    }
  }
  row = -1;
  return false;
}

/*Q2
Write a function mirror_label(board, height, width, row, column) which
returns the alphabetical character label (if any) attached to the mirror found at
coordinates (row, column) within the height × width 2D array of characters
board. 
If there is no mirror at the coordinates, or if there is no alphabetical
character attached then the function should return the character ’\0’.
*/

char mirror_label(char** board, int height, int width, int row, int col) {
  if (board[row][col]!='/' && board[row][col]!='\\'){
    return '\0';
  }

  if (board[row][col]=='/'){
    if (isalpha(board[row-1][col+1])){
      return board[row-1][col+1];
    }
    else if (isalpha(board[row+1][col-1])){
      return board[row+1][col-1];
    }
  }
  else if (board[row][col]=='\\'){
    if (isalpha(board[row+1][col+1])){
      return board[row+1][col+1];
    }
    else if (isalpha(board[row-1][col-1])){
      return board[row-1][col-1];
    }
  }

  return '\0';
}

void move_beam(char** board, int& row, int& col, Direction dir){
  char current_symbol = board[row][col];

  if (current_symbol== '-' || current_symbol== '|' || current_symbol== '+'){
    board[row][col] = '+';
  }
  else if (current_symbol!= '\\' && current_symbol!= '/' && current_symbol!= '>' ){
    switch(dir){
      case NORTH: case SOUTH: board[row][col]='|'; break;
      case EAST: case WEST: board[row][col]='-';
    }
  }

  switch(dir){
    case NORTH: row--; break;
    case SOUTH: row++; break;
    case EAST: col++; break;
    case WEST: col--; break;
  }
  return;
}

void update_direction(const char current_symbol, Direction& dir){
  if (current_symbol =='/'){
    switch(dir){
      case EAST: dir = NORTH; return;
      case SOUTH: dir = WEST; return;
      case WEST: dir = SOUTH; return;
      case NORTH: dir = EAST; return;
    }
  }
  else if (current_symbol =='\\'){
    switch(dir){
      case EAST: dir = SOUTH; return;
      case SOUTH: dir = EAST; return;
      case WEST: dir = NORTH; return;
      case NORTH: dir = WEST; return;
    }
  }
}

/*Q3
Write a Boolean function shoot(board, height, width, message, last row,
last col) which plots the path of the laser beam across the given board,
updating board and output string message appropriately. 
The output parameters last row and last col should be set to the final coordinates of the
laser beam, which should terminate:
- at an edge,
- the laser light absorber or 
- a ’?’. 
The function should return true if and only if the final coordinates of
the laser beam correspond to the laser light absorber.
*/

// enum Direction {NORTH, EAST, SOUTH, EAST};

bool shoot(char** board, int height, int width, char* message, int& last_row, int& last_col){
  
  if (!find_laser(board, height, width, last_row)){
    return false;
  }
  last_col = 0;
  Direction dir = EAST;

  while(true){
    // simply move the beam without conducting any checks
    move_beam(board, last_row, last_col, dir);
    //cout << last_row << " " << last_col << endl;
    char current_symbol = board[last_row][last_col];

    if (current_symbol=='?' || current_symbol=='#' || current_symbol=='@'){
      // terminate
      break;
    }

    if (current_symbol=='\\' || current_symbol=='/'){
      // store the character
      char label = mirror_label(board, height, width, last_row, last_col);

      if (label != '\0' && isalpha(label)){
        *message = label;
        message++;
      }
      // update the direction
      update_direction(current_symbol, dir);
    }
  }

  *message='\0';
  return (board[last_row][last_col]=='@');
}


/*
Write a recursive Boolean function solve(board, height, width, target)
which takes in a board containing ’?’ characters and replaces each of these by
a ’/’, a ’\’ or a ’ ’ (space) in such a way that the laser beam (a) accumulates
the message given by input string target and (b) terminates at the laser light
absorber. If these objectives can be met, the function should return true;
otherwise the function should return false.
For full credit for this part, your function should be recursive.
*/

char SYMBOLS[4]= {'/', '\\', ' ', '\0'};

void clean_board(char** board, int height, int width){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (board[r][c] == '-' || board[r][c] == '|' || board[r][c] == '+'){
        board[r][c] = ' ';
      }
    }
  }
}

bool solve(char** board, int height, int width, const char* target){
  int row, col;
  char message[512];

  if (shoot(board, height, width, message, row, col)){
    if (strcmp(target, message)==0){
      clean_board(board, height, width);
      return true;
    }
  }
  clean_board(board, height, width);

  if (board[row][col]=='?'){
    // we are still at the game
    for (int i = 0; SYMBOLS[i]; i++){
      board[row][col]=SYMBOLS[i];
      if (solve(board, height, width, target)){
        return true;
      }
    }
    board[row][col]='?';
  }
  clean_board(board, height, width);
  return false;
}