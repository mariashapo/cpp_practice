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
bool get_position(char** board, const char ch, int& row, int& column){
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (board[r][c]==ch){
        row = r; column = c;
        return true;
      }
    }
  }
  row = -1; column = -1;
  return false;
}

// overriden simplified function get_position
bool get_position(char** board, const char ch){
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (board[r][c]==ch){
        return true;
      }
    }
  }
  return false;
}

bool two_letter_adjacent(char** board, const char letter_one, const char letter_two){
  int row_one, col_one, row_two, col_two;
  get_position(board, letter_one, row_one, col_one);
  get_position(board, letter_two, row_two, col_two);
  if (abs(row_one-row_two)>1 || abs(col_one-col_two)>1){
    return false;
  }
  return true;
}

bool word_formed(char** board, const char* word){

  for (const char* letter = word; *(letter+1); letter++){
    if (!two_letter_adjacent(board, *letter, *(letter+1))){
      return false;
    }
  }
  return true;
}

bool unique_letters(char** board){
  for (char ch = 'A'; ch < 'Z'; ch++){
    if (!get_position(board, ch)){
      return false;
    }
  }
  return true;
}

bool valid_solution(char** board, char** words){
  for (char** word = words; *word; word++){
    if (!word_formed(board, *word)){
      return false;
    }
  }
  if (!unique_letters(board)){
    return false;
  }
  return true;
}

/*
- [X] If ch is found in the board at position (r, c), then set every entry in mask to be false with the
exception of element (r, c), which should be set to true, and return from the function.
- [X] For every cell in the board that is marked with a letter (that is not ch), set the corresponding
element in mask to false.
- [X] If there is one (and only one) cell with the value true in mask then set the corresponding cell
in the board to ch
*/

void update(char** board, const char ch, Mask& mask){
  int row, col;
  if (get_position(board, ch, row, col)){
    mask.set_all(false);
    mask.set_element(row, col, true);
    return;
  }

  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (isalpha(board[r][c])){
        mask.set_element(r, c, false);
      }
    }
  }

  if (mask.count()==1){
    mask.get_position(true, row, col);
    board[row][col]=ch;
  }

  return;

}

/*
[X] Write a (void) function neighbourhood intersect(one, two) which modifies masks one and one
by intersecting each with the 1-neighbourhood of the other
*/

void neighbourhood_intersect(Mask& one, Mask& two){

  Mask one_1d = one.neighbourhood(1);
  Mask two_1d = two.neighbourhood(1);

  one.intersect_with(two_1d);
  two.intersect_with(one_1d);
}

int count_board_letters(char** board){
  int count = 0;
  for (int row = 0; row<HEIGHT; row++){
    for (int col = 0; col < HEIGHT; col++){
      if (isalpha(board[row][col])){
        count++;
      }
    }
  }
  return count;
}

/*
[X] Create an array of 25 masks (one for each letter ’A’ to ’Y’) and use the update(...) function to
initialise them.
[X] Then repeatedly use the adjacent letters appearing in each word with the neighbourhood intersect(...)
and update(...) functions to refine the masks of free letters as much as possible.
[X] Finally, if it necessary, recursively investigate the remaining options for the free letters. This should
not be necessary with the “easy” puzzle, but will be needed for the “medium” and “hard” puzzles.
*/

bool solve_board_recursive(char** board, char** words){
  int row, col;
  if (valid_solution(board, words)){
    return true;
  }
  // find next empty
  if (!get_position(board, '.', row, col)){
    return false;
  }

  for (char ch = 'A'; ch < 'Z'; ch++){
    if (get_position(board, ch)){continue;}
    board[row][col] = ch;
    if (solve_board_recursive(board, words)){return true;}
    else{continue;}
  }
  board[row][col] = '.';
  return false;
}

bool solve_board(char** board, char** words){
  Mask masks [25];
  char ch = 'A';
  int number_of_letters = 0;

  for (int offset = 0; offset<25; offset++){
    update(board, ch+offset, masks[offset]);
  }

  while (count_board_letters(board)-number_of_letters>0){
    number_of_letters = count_board_letters(board);
    for (char** word = words; *word; word++){ // for each word
      for (const char* letter = *word; *(letter+1); letter++){ // investigate each letter pair
        neighbourhood_intersect(masks[*letter-'A'], masks[*(letter+1)-'A']);
        update(board, *letter, masks[*letter-'A']);
        update(board, *(letter+1), masks[*(letter+1)-'A']);
      }
    }
  }

  solve_board_recursive(board, words);

  if (valid_solution(board, words)){
    return true;
  }

  return false;  
}