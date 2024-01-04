#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

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

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  // get the first line
  input.getline(line,512);  

  while (input) {
    if ( (int) strlen(line) > width)
      // adjust the width to be the max line
      width = strlen(line);
    
    // each row increment the height
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
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
      cout << m[r][c];
    cout << endl;
  }
}

/* my functions */

// Q1
/*“barrier” characters ’-’, ’+’ and ’|’ for hedges
(unique) “marker” characters to denote points
of interest; here ’>’ represents the entrance, 
’X’ the exit, and 
’U’ an unreachable square.*/


//eg: find_marker('>', maze, height, width, row, column);

bool find_marker(const char ch, char** maze, int height, int width, int& row, int& col){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (maze[r][c] == ch){
        row = r; col = c;
        return true;
      }
    }
  }
  row = -1; col = -1;
  return false;
}

// Q2
// {'N', 'E', 'S', 'W'}
bool move(const char dir, int& row, int& col){
  switch(dir){
   case 'N': row--; return true; 
   case 'S': row++; return true;
   case 'W': col--; return true;
   case 'E': col++; return true;
   default: return false;
  }
}

bool bounds_check(int row, int col, int h, int w){
  if (row<0 || col < 0){
    return false;
  }
  else if (row >= h || col >= w){
    return false;
  }
  else{
    return true;
  }
}

bool make_move(char** maze, const char direction, int h, int w, int& row, int& col){

  if (!move(direction, row, col)){
    cerr << "Invalid direction";
    return false;
  }
  // otherwise row and col are updated

  //  without moving outside the boundaries of the maze
  if (!bounds_check(row, col, h, w)){
    return false;
  }
  // barrier” characters ’-’, ’+’ and ’|’ for hedges
  if (maze[row][col] == '-' || maze[row][col] == '+' || maze[row][col] == '|'){
    return false;
  }
  return true;
}

/*Write a function valid solution(path, maze, height, width)
which determines 
- [X] if a given path through a height × width maze
leads from the entrance marker ’>’ to the exit marker ’X’ 
- [X] without moving outside the boundaries of the maze 
- [X] or passing through a hedge. 
* The parameter path is a string of uppercase characters,
each of which is in the set {’N’,’S’,’E’,’W’} (corresponding to
North, South, East and West respectively).*/

// eg: valid_solution("EEEEESEENN", maze, height, width)
bool valid_solution(const char* path, char** maze, int height, int width, const char start_char, const char end_char){
  int row, col;
  if (!find_marker(start_char, maze, height, width, row, col)){
    // start marker could not be found
    return false;
  }
  // /if ()
  for (const char* c = path; *c; c++){
    if (!make_move(maze, *c, height, width, row, col)){
      return false;
    }
  }
  // completed all the move. so should be at the last character
  if (maze[row][col] == end_char){
    return true;
  }
  else{
    return false;
  }
}

//Q3 
/*
Write a function find path(maze, height, width, start, end)
which finds a valid solution path through a height × width maze
beginning at marker character start and terminating at marker
character end. 
[X] The path should be marked on the maze using ’#’
characters, and the function should return a string of direction
movements. 
[X] If there is no path, the function should return the
string "no solution".
*/
//eg: find_path(maze, height, width, '>', 'X')

bool find_path_recursive(char** maze, int height, int width, const char start_marker, const char end_marker, char* path, int row, int col){
  if (maze[row][col]==end_marker){ // && valid_solution(path, maze, height, width, start_marker, end_marker
    *path = '\0'; // terminate the path string
    return true;
  }

  int row_before = row, col_before = col;
  for (const char* dir = DIRECTIONS; *dir; dir++){
    
    if (!make_move(maze, *dir, height, width, row, col)){
      row = row_before; col = col_before;
      continue;
      }
    if (maze[row][col] == '#' || maze[row][col] == start_marker){
      row = row_before; col = col_before;
      continue;
      }
    // the move is valid
    // fill in with # and store the move
    *path = *dir;
    if (maze[row][col] != end_marker){
      maze[row][col] = '#';
    }
    if (find_path_recursive(maze, height, width, start_marker, end_marker, path+1, row, col)){
      return true;
    }
    else{
      // undo the move
      maze[row][col] = ' ';
      // keep track of the position
      row = row_before; col = col_before;
      // remove the last path letter
      *path = '\0';
    }
  }

  return false;
}

const char* find_path(char** maze, int height, int width, const char start_marker, const char end_marker){
  char* path = new char[512];
  path[0] = '\0';

  int row, col;
  find_marker(start_marker, maze, height, width, row, col);
  
  if (find_path_recursive(maze, height, width, start_marker, end_marker, path, row, col)){
    return path;
  }
  else{
    strcpy(path, "no solution");
    return path;
  }
} 