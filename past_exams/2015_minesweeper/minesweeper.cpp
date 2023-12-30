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


// rue if and only if all non-mine squares in the playing board have been uncovered
bool is_complete(const char mines[9][9], const char revealed[9][9]){
  // non-mine square == ' '
  for (int r = 0; r<9; r++){
    for (int c = 0; c<9; c++){
      if (mines[r][c]=='.' && revealed[r][c]=='?'){
          return false;
      }
    }
  }
  return true;
}

void convert_position(const char* position, int &row, int &col){
  row = position[0]-'A';
  col = position[1]-'1';
}

// return true if an a
int is_flag_move(const char* position){
  if (position[2]=='\0'){
    return 0;
  }
  else if (position[2]=='*'){
    return 1;
  }
  return -1;
}

int count_mines(const char* position, const char mines[9][9]){
  int row, col, count = 0;
  convert_position(position, row, col);

  for (int r = -1; r < 2; r++){
    for (int c = -1; c < 2; c++){
      if (row + r < 0 || col +c < 0 || row + r >8 || col + c >8){
        continue;
      }
      else if (c == 0 && r == 0){
        continue;
      }
      else if (mines[row + r][col + c]=='*'){
        count++;
      }
    }
  }
  return count;
}

int count_obj(int row, int col, const char board[9][9], const char my_char) {
  int count = 0;
  for (int r = -1; r <= 1; r++) {
    for (int c = -1; c <= 1; c++) {
      int newRow = row + r, newCol = col + c;
      if (newRow < 0 || newCol < 0 || newRow >= 9 || newCol >= 9 || (r == 0 && c == 0)) {
        continue;
      }
      if (board[newRow][newCol] == my_char) {
        count++;
      }
    }
  }
  return count;
}

MoveResult make_move(const char* position, const char mines[9][9], char revealed[9][9]){
  int row, col;
  bool flag_move;
  //cout << "<" << position << ">"<< endl;
  convert_position(position, row, col);
  int flag = is_flag_move(position);

  flag_move = (flag==1);

  if (flag == -1){
    return INVALID_MOVE;
  }
  //cout << "row: " << row << " and col: " << col << endl;
  if (revealed[row][col]!='?'){
    return REDUNDANT_MOVE;
  }
  
  if (!flag_move){ // then uncover
    if (mines[row][col]=='*'){
      return BLOWN_UP;
    }
    uncover(row, col, mines, revealed);
  }
  else{
    // put a flag
    revealed[row][col]='*';
  }

  if (is_complete(mines, revealed)){
    return SOLVED_BOARD;
  }
  else{
    return VALID_MOVE;
  } 
}

// keep uncovering until we reach a non ' ' square
void uncover(int row, int col, const char mines[9][9], char revealed[9][9]){

  int num_mines = count_obj(row, col, mines, '*');
  // if not ' ', put the number in and return

  if (num_mines != 0){
    // put the number in
    revealed[row][col] = static_cast<char>(num_mines+'0');
    return;
  }
  else{
    revealed[row][col] = ' ';
  }

  // try all possible squares around
  for (int r = -1; r < 2; r++){
    for (int c = -1; c < 2; c++){
      if (row + r < 0 || col + c < 0 || row + r >8 || col + c >8){
        continue;
      }
      else if (c == 0 && r == 0){
        continue;
      }
      else if (revealed[row + r][col + c]=='?'){
        uncover(row + r, col +c, mines, revealed);
      }
    }
  }
  return;
}

void form_position(char* position, int row, int col, bool flag = false){
  position[0]=static_cast<char>(row+'A');
  position[1]=static_cast<char>(col+'1');
  if (flag){
    position[2]='*';
  }
  else{
    position[2]='\0';
  }
  return;
}

bool is_mine(int row, int col, const char revealed[9][9]){
  char my_char;
  int count, newRow, newCol;
  for (int r = -1; r < 2; r++){
    for (int c = -1; c < 2; c++){
      newRow = row + r; newCol = col + c;
      if (newRow < 0 || newCol < 0 || newRow >8 || newCol >8 || (c == 0 && r == 0)){
        continue;
      }
      // number at one of the adjacent positions
      my_char = revealed[newRow][newCol];
      if (isdigit(my_char)){
        count = count_obj(newRow, newCol, revealed, '?') + count_obj(newRow, newCol, revealed, '*');
        if (count == my_char - '0'){
          return true;
        }
      }
    }
  }
  return false;
}

bool is_safe(int row, int col, const char revealed[9][9]){
  int newRow, newCol;
  char my_char;
  for (int r = -1; r < 2; r++){
    for (int c = -1; c < 2; c++){
      newRow = row + r; newCol = col + c;
      if (newRow < 0 || newCol < 0 || newRow >8 || newCol >8 || (c == 0 && r == 0)){
        continue;
      }
      my_char = revealed[newRow][newCol];
      if (isdigit(my_char)){
        // if there is an adjacent square that is touching its own number of '*', the rest must be safe
        if (count_obj(newRow, newCol, revealed, '*') == my_char - '0'){
          return true;
        }
      }
    }
  }
  return false;
}

bool find_safe_move(char revealed[9][9], char* move){
  // safe position
  for (int row = 0; row < 9; row ++){
    for (int col = 0; col < 9; col ++){
      if (revealed[row][col]=='?'){
        if (is_safe(row, col, revealed)){
          form_position(move, row, col);
          return true;
        }
        else if (is_mine(row, col, revealed)){
          form_position(move, row, col, true);
          return true;
        }
        else{
          continue;
        }
      }
    }
  }
  return false;
}