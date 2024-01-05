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

/*Write a function make river scene(left, boat) which takes
two character strings: left describing the contents of the left
river bank, and boat describing the contents of the boat2, and
which returns a corresponding ASCII-art scene. 
[X] The character string for the left bank may be up to 7 letters long and may con-
tain three different kinds of characters: M denoting the presence
of a missionary, C denoting the presence of a cannibal and B de-
noting that the boat is at the left bank. 
[X] The character string for the boat may be up to 2 letters long and may contain only the
characters M and C. Note both strings may also be empty and the
order of the characters is irrelevant. 

[X] Hint: Start by drawing the
left river bank at (0,0), the right river bank at (0,53) the sun at
(3,30), and the river at (19,19)3.*/

//eg: make_river_scene("CM","MM");

// set the scene baseline
bool set_scene(char** scene){
  bool status = true;
  status = status && add_to_scene(scene, 0, 0, "bank.txt");

  status = status && add_to_scene(scene, 0, 52, "bank.txt");

  status = status && add_to_scene(scene, 3, 30, "sun.txt");

  status = status && add_to_scene(scene, 19, 19, "river.txt");

  return status;
}

int count_characters(const char* str, const char symbol){
  int count = 0;
  for (const char* c = str; *c; c++){
    if (*c == symbol){
      count++;
    }
  }
  return count;
}

bool read_bank_input(const char* str, int& missionary, int& cannibal, int& boat){
  if (strlen(str)>7){
    return false;
  }
  missionary = count_characters(str, 'M');
  cannibal = count_characters(str, 'C');
  boat = count_characters(str, 'B');
  if (missionary > 3 || cannibal > 3 || boat > 1){
    return false;
  }
  return true;
}

bool read_boat_input(const char* str, int& missionary, int& cannibal){
  if (strlen(str)>2){
    return false;
  }
  missionary = count_characters(str, 'M');
  cannibal = count_characters(str, 'C');
  return true;
}

void set_bank(char **scene, int missionary, int cannibal, int boat, char side){
  int x_location = (side == 'l')?1:53;
  for (int m = 0; m < missionary; m++){
    add_to_scene(scene, 2, x_location, "missionary.txt");
    x_location = x_location + 6;
  }
  // reset x_location
  x_location = (side == 'l')?1:53;
  for (int c = 0; c < cannibal; c++){
    add_to_scene(scene, 11, x_location, "cannibal.txt");
    x_location = x_location + 6;
  }
  // reset x_location for the boat
  x_location = (side == 'l')?19:35;
  if (boat){
    add_to_scene(scene, 17, x_location, "boat.txt");
  }
}


void set_boat(char **scene, int missionary, int cannibal, int b_left){
  int x_location = (b_left)?22:38;
  for (int m = 0; m < missionary; m++){
    add_to_scene(scene, 11, x_location, "missionary.txt");
    x_location = x_location + 6;
  }

  for (int c = 0; c < cannibal; c++){
    add_to_scene(scene, 11, x_location, "cannibal.txt");
    x_location = x_location + 6;
  }
}

char** make_river_scene(const char* left, const char* boat){
  int m_left, c_left, b_left, m_boat, c_boat;
  int m_right, c_right, b_right;
  // create an empty scene
  char** scene = create_scene();
  
  if (!set_scene(scene)){
    cerr << "issue reading one of the files" << endl;
  }
  // obtain the number of characters ont the left bank
  if (!read_bank_input(left, m_left, c_left, b_left)){
    cerr << "invalid left string" << endl;
  }
  if (!read_boat_input(boat, m_boat, c_boat)){
    cerr << "invalid boat string" << endl;
  }

  // obtain the count for the rest of the characters
  m_right = 3 - m_left - m_boat;
  c_right = 3 - c_left - c_boat;
  b_right = 1 - b_left;

  set_bank(scene, m_left, c_left, b_left, 'l');
  set_bank(scene, m_right, c_right, b_right, 'r');
  set_boat(scene, m_boat, c_boat, b_left); 
  // b_left is to indicate whether the boat is on the left or not

  return scene;
}

/*Write a function perform crossing(left, targets) which per-
forms a crossing of the river by one or two target entities4 from one
bank of the river to the other using the boat. 

The parameter left is both an input and an output parameter: it initially describes
the contents of the left river bank, but is modified to reflect the
result of the move (including an update on the boat’s position).

The parameter targets is a string with one or two characters
drawn from the letters M and C according to the entities perform-
ing the crossing (N.B. performing a “crossing” without anyone on
the boat is not allowed).*/

bool remove_targets(char* str,const char* targets){
  char* to_remove;
  for (const char* target = targets; *target; target++){
    to_remove = strchr(str, *target);
    if (to_remove == NULL){
      return false;
    }
    for (char* start = to_remove; *start; start++){
      *start = *(start + 1);
    }
  }
  return true;
}

int check_counts(int m_l, int c_l, int m_b, int c_b){
  int m_r, c_r;

  m_r = 3 - m_l - m_b;
  c_r = 3 - c_l - c_b;

  if (m_l > 3 || m_l < 0 || m_r > 3 || m_r < 0){
    return ERROR_INVALID_MISSIONARY_COUNT;
  }
  if (c_l > 3 || c_l < 0 || c_r > 3 || c_r < 0){
    return ERROR_INVALID_CANNIBAL_COUNT;
  }
  if ((m_l < c_l && m_l > 0) || (m_r < c_r && m_r > 0)){
    return ERROR_MISSIONARIES_EATEN;
  }
  return 1;
}

int perform_crossing(char* left, const char* targets, bool print){
  int status;
  int m_left, c_left, b_left, m_boat, c_boat;
  char** scene;
  // obtain the number of characters on the left bank
  read_bank_input(left, m_left, c_left, b_left);
  if (!read_boat_input(targets, m_boat, c_boat)){
    return ERROR_INVALID_MOVE;
  }

  status = check_counts(m_left, c_left, 0, 0);
  if (status != 1){
    return status;
  }

  if (m_boat + c_boat == 0){
    return ERROR_INVALID_MOVE;
  }


  //(a) Loading the boat:
  // if boat is on the left, we have to remove targets from the left
  if (print){cout << "Loading the boat..."  << endl;}
  if (b_left){
    c_left -= c_boat;
    m_left -= m_boat;

    status = check_counts(m_left, c_left, m_boat, c_boat);
    if (status != 1){
      return status;
    }
    remove_targets(left,targets);
  }
  
  if (print){
    scene = make_river_scene(left,targets);
    print_scene(scene);  
    destroy_scene(scene);
  }

  //(b) Crossing the river
  if (print){cout << "Crossing the river..." << endl;}
  if (b_left){
    remove_targets(left,"B");
    b_left--;
  }
  else{
    strcat(left,"B");
    b_left++;
  }
  
  if (print){
    scene = make_river_scene(left,targets);
    print_scene(scene);  
    destroy_scene(scene);
  }
  
  //(c) Unloading the boat
  if (print){cout << "Unloading the boat..." << endl;}
  // if boat arrived to the left bank
  if (b_left){
    strcat(left, targets);
    m_left += m_boat;
    c_left += c_boat;
  }
  
  status = check_counts(m_left, c_left, 0, 0);
  if (status != 1){
    return status;
  }
  
  if (print){
    scene = make_river_scene(left,"");
    print_scene(scene);  
    destroy_scene(scene);
  }

  if (*left == '\0'){
    return VALID_GOAL_STATE;
  }

  return VALID_NONGOAL_STATE;
}

/*Write a function play game() which allows a user to play the
game of missionaries and cannibals by suggesting boat crossings
via the keyboard. The return value of the function should be
an appropriate status code e.g. ERROR MISSIONARIES EATEN or
VALID GOAL STATE (i.e. everyone crossed successfully).*/

int play_game(){
  int result;
  char** scene;
  char target[3], left[8];
  strcpy(left, "BCCCMMM");

  scene = make_river_scene(left,"");
  print_scene(scene);  
  destroy_scene(scene);

  while (true){
    cout << "Input up to 2 figures to move: ";
    cin >> target;
    result = perform_crossing(left, target);
    if (result != VALID_NONGOAL_STATE){
      return result;
    }
  }
}

void get_last_move(char* answer, char* move){
    char* last_comma = strrchr(answer, ',');
    if (last_comma != NULL) {
        strcpy(move, last_comma+1);
    } else {
        strcpy(move, answer);
    }
}

void remove_last_move(char* answer) {
    char* last_comma = strrchr(answer, ',');
    if (last_comma != NULL) {
        *last_comma = '\0';
    } else {
        answer[0] = '\0';
    }
}

void add_move(const char* move, char* answer) {
    if (isalpha(answer[0])) {  // Check if answer is not empty
        strcat(answer, ",");
    }
    strcat(answer, move);
}


const char* MOVES[6] = {"M", "MC", "CC", "MM", "C", NULL};

int find_solution(char* left, char* answer, int move_count){
  char left_before_move[8], previous_move[3];
  int result;
  if (move_count == 0){
    if (answer[0]!='\0'){
      cout << answer << endl;
      cout << "interesting" << endl;
    }
    answer[0] = '\0';
  }
  // success termination check
  if (!*left){
    return VALID_GOAL_STATE;
  }
  else if(move_count >=50){
    return ERROR_BONUS_NO_SOLUTION;
  }
  strcpy(left_before_move, left);
  for(int i = 0; MOVES[i]; i++){
    get_last_move(answer, previous_move);
    if (strcmp(previous_move, MOVES[i]) == 0){continue;}

    result = perform_crossing(left, MOVES[i], false);
    if (result > 0){

      add_move(MOVES[i], answer);
      
      if (find_solution(left, answer, move_count + 1) == VALID_GOAL_STATE){
        return VALID_GOAL_STATE;
      }
      remove_last_move(answer);
    }
    strcpy(left, left_before_move);
  }

  return ERROR_BONUS_NO_SOLUTION; 
}
// MC,M,CC,C,MM,MC,MM,C,CC,M,MC
/*
result = perform_crossing(char* left, const char* targets);
get_last_move(move, answer);
if (result == VALID_GOAL_STATE){
  return VALID_GOAL_STATE;
}
else if(result != VALID_NONGOAL_STATE){
  return ERROR_INVALID_MOVE;
}
*/