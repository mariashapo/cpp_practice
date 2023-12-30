#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <vector>


using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++){
        cout << scene[i][j];
    }
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
// this function basically cleans up the junk remainders and places /0
void filter(char *line) {
  // starting with some *line
  while (*line) {
    if (*line >= ' ')
    // space is the first ascii character
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  // the line we are getting from the input file
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	      return false;
      if (col >= SCENE_WIDTH)
	      break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

// make a river scene based on the characters on the left bank and on the boat
void add_river_base(char** scene);

void full_scene(vector <char>& my_vect){
  int index = 0;
  for (int c = 0; c<3; c++){
    my_vect[index]='C';
    index++;
  }
  for (int m = 0; m<3; m++){
    my_vect[index]='M';
    index++;
  }
  my_vect[index]='B';
}

void remove_char_from_vect(vector <char>& vect_modify, char char_to_del){
  int index = 0;
  for (const char& c: vect_modify){
    if (c==char_to_del){
      int pos = index;
      vect_modify.erase(pos);
      return;
    }
    index++;
  }
}

void remove_vect_from_vect(vector <char>& vect_modify, const vector <char>& vect_const){

}

char** make_river_scene(const char* left, const char* boat){

  vector <char> left_v, right_v, boat_v;

  // is there a faster way to conver a c-string into a vector?
  for (const char *c = left; *c; c++){
    left_v[left-c]=*c;
  }

  char** scene = create_scene();
  add_river_base(scene);

  return scene;
}

void add_river_base(char** scene){
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
}