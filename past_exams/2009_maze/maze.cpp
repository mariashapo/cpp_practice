#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

const char directions[] = {'N', 'E', 'S', 'W'};

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

bool find_marker(char marker, char** maze, int height, int width, int& row, int& column){
  for (int i = 0; i<height; i++){
    for (int j = 0; j<width; j++){
      if (maze[i][j]==marker){
        row = i;
        column = j;
        return true;
      }
    }
  }
  row = -1;
  column = -1;
  return false;
}

bool valid_solution(const char* path, char** maze, int height, int width, char start_marker, char end_marker){
  // eg: EEEEESEENN
  int row = 0, col = 0;
  find_marker(start_marker, maze, height, width, row, col);
  for (const char *c = path; *c; c++){
    if (moveMaze(*c, height, width, row, col)){
      if (maze[row][col]==end_marker){
        return true;
      }
      else if (maze[row][col]!=' '){
        return false;
      }
    }
    else{
      // moved outside of the borders
      return false;
    }
  }
  return false;
}

bool moveMaze(const char direction, int height, int width, int& row, int& col){
  switch(direction){
    case 'N':
      return --row >= 0;
    case 'S':
      return ++row <= (height-1);
    case 'E':
      return ++col <= (width-1);
    case 'W':
      return --col >= 0;
    default:
      return false;
  }
}

bool validMoveMaze(char** maze, const char direction, int height, int width, int& row, int& col, char end_marker){
  int row_before = row;
  int col_before = col;
  if (moveMaze(direction, height, width, row, col) && (maze[row][col]==' ' || maze[row][col]==end_marker)){
    return true;
  }
  else{
    row = row_before;
    col = col_before;
    return false;
  }
}

const char* find_path(char** maze, int height, int width, char start_marker, char end_marker){
  int row_start, col_start;

  find_marker(start_marker, maze, height, width, row_start, col_start);
  char* str = new char[height*width];
  cout << "here" << endl;
  bool found = find_solution(maze, str, height, width, row_start, col_start, end_marker);

  if (found){
    return str;
  }
  else{
    return "no solution";
  }
}

bool find_solution(char** maze, char* str, int height, int width, int current_row, int current_col, char end_marker){
    if (maze[current_row][current_col] == end_marker) {
        *str = '\0';
        return true;
    }

    for (const char dir : directions) {
        int next_row = current_row, next_col = current_col;

        if (!validMoveMaze(maze, dir, height, width, next_row, next_col, end_marker)) {
            continue; // Skip to the next direction if move is invalid
        }

        maze[current_row][current_col] = '#'; // Mark the path
        *str = dir;

        if (find_solution(maze, str + 1, height, width, next_row, next_col, end_marker)) {
            return true; // Successful path found
        }

        // Unmark the path if not successful
        maze[current_row][current_col] = ' ';
    }

    *str = '\0'; // Mark as dead end
    return false; // No valid path found
}

