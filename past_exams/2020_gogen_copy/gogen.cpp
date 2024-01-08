#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    // read in line by line, each line - one row
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */

// Q1
/*
Write a Boolean function get position(board, ch, row, column) which searches for the first occur-
rence of character ch in a given board when searched in a row-by-row fashion. If the character is found,
the function should return true and set the output parameters row and column to the row and column
number (indexed starting from 0). If the character cannot be found, the function should return false
and row and column should both be set to -1.
*/

// get_position(board, 'B', row, column)
bool get_position(char** board, const char ch, int& row, int& col){
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (board[r][c] == ch){
        row = r;
        col = c;
        return true;
      }
    }
  }
  row = -1;
  col = -1;
  return false;
}

//Q2
/*
Write a Boolean function valid solution(board, words) which returns true if the given board repre-
sents a solution to the Gogen puzzle with the given list of words.
*/

/*
As shown on the left of Figure 1, a Gogen puzzle consists of a 5 × 5 board and a list of words. As shown on
the right of Figure 1, the aim of Gogen is to insert the 25 letters ’A’ to ’Y’ (’Z’ is not used) into the cells
on the board such that (a) each letter appears in one (and only one) board cell, and (b) by moving one step
(horizontally, vertically or diagonally) at a time, it is possible to spell out each of the words.
*/

bool unique_letters(char** solution){
  int row, col;
  for (char c = 'A'; c<'Z';c++){
    if (!get_position(solution, c, row, col)){
      return false;
    }
  }
  return true;
}

bool can_form_word(char** solution, char* word){
  int row, col, row_before, col_before;
  get_position(solution, word[0], row_before, col_before);

  for (int i = 0; word[i]; i++){
    get_position(solution, word[i], row, col);
    if (abs(row - row_before)>1 || abs(col - col_before)>1){
      return false;
    }
    row_before = row; col_before = col;
  }
  return true;
}

bool can_form_words(char** solution, char** words){
  for (char** word = words; *word; word++){
    if (!can_form_word(solution, *word)){
      return false;
    }
  }
  return true;
}

bool valid_solution(char** solution, char** words){
  // check if the letters are unique
  if (!unique_letters(solution)){
    return false;
  }
  // check if all the words can be formed
  if (!can_form_words(solution, words)){
    return false;
  }
  return true;
}

//Q3
/*
(a) Write a (void) function update(board, ch, mask) which mutually updates the board and a mask
for letter ch according to the following rules:
[X] If ch is found in the board at position (r, c), then set every entry in mask to be false with the
exception of element (r, c), which should be set to true, and return from the function.
[X] For every cell in the board that is marked with a letter (that is not ch), set the corresponding
element in mask to false.
[X] If there is one (and only one) cell with the value true in mask then set the corresponding cell
in the board to ch.
*/

void update(char** board, const char ch, Mask& mask){
  int row, col;
  if (get_position(board, ch, row, col)){
    mask.set_all(false);
    mask.set_element(row, col, true);
    return;
  }
  // set all mask positions that have characters in the board to false
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (isalpha(board[r][c])){
        mask.set_element(r, c, false);
      }
    }
  }
  if(mask.count() == 1){
    mask.get_position(true, row, col);
    board[row][col] = ch;
  }
}

/*
(b) Write a (void) function neighbourhood intersect(one, two) which modifies masks one and one
by intersecting each with the 1-neighbourhood of the other.
*/

void neighbourhood_intersect(Mask& one, Mask& two){
  Mask one_neighb = one.neighbourhood();
  Mask two_neighb = two.neighbourhood();

  one.intersect_with(two_neighb);
  two.intersect_with(one_neighb);

  return;
}


/*
Write a Boolean function solve board(board, words) which attempts to find a solution to a given
Gogen puzzle. If a solution can be found, parameter board should contain the completed board. Oth-
erwise the function should return false. A high-level overview of one possible approach is:
[X] Create an array of 25 masks (one for each letter ’A’ to ’Y’) and use the update(...) function to
initialise them.
[X] Then repeatedly use the adjacent letters appearing in each word with the neighbourhood_intersect(...)
and update(...) functions to refine the masks of free letters as much as possible.
[ ] Finally, if it necessary, recursively investigate the remaining options for the free letters. This should
not be necessary with the “easy” puzzle, but will be needed for the “medium” and “hard” puzzles.
*/

int count_letters(char** board){
  int count = 0;
  for (int row = 0; row < HEIGHT; row++){
    for (int col = 0; col < WIDTH; col++){
      if (isalpha(board[row][col])){
        count++;
      }
    }
  }
  return count;
}

bool solve_board(char** board, char** words){
  int count_before = 0, count_after = 0;
  if (valid_solution(board, words)){
    return true;
  }

  Mask masks[25];
  // create a mask for each letter
  int i = 0;
  for (char ch = 'A'; ch < 'Z'; ch++, i++){
    update(board, ch, masks[i]);
  }

  do{
    count_before = count_letters(board);
    for (int r = 0; words[r]; r++){
      for (int c = 0; words[r][c+1]; c++){
        // words[r][c] is a certain letter
        char letter_one = words[r][c];
        char letter_two = words[r][c+1];
        int mask_one_position = letter_one - 'A';
        int mask_two_position = letter_two - 'A';
        neighbourhood_intersect(masks[mask_one_position], masks[mask_two_position]);
        for (char ch = 'A'; ch < 'Z'; ch++){
          update(board, ch, masks[ch - 'A']);
        }
        count_after = count_letters(board);
      }
    }

  } while(count_before < count_after);
  
  if (valid_solution(board, words)){
    return true;
  }
  else{
    return solve_board_recursive(board, words);
  }
}

bool solve_board_recursive(char** board, char** words){
  int row, col, row_temp, col_temp;
  if (valid_solution(board, words)){
    return true;
  }
  // find first empty
  if (!get_position(board, '.', row, col)){
    return false;
  }
  for (char ch = 'A'; ch < 'Z'; ch++){
    // letter is already present
    if (get_position(board, ch, row_temp, col_temp)){
      continue;
    }
    board[row][col] = ch;
    if (solve_board_recursive( board, words)){
      return true;
    }
    // undo the move
    board[row][col] = '.';
  }
  return false;
}