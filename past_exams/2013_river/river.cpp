#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>


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

void add_river_base(char** scene){
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
}

void insert_traveller(char** scene, int vertical_pos, int offset, const char* filename){
  for (int i = 0; i<3; i++){
    if (scene[vertical_pos+3][1 + offset + i*6]==' '){
      add_to_scene(scene, vertical_pos, 1 + offset + i*6, filename);
      break;
    }
  }
}

void insert_boat(char** scene, int offset){
  add_to_scene(scene, 17, 19+offset, "boat.txt");
}

void populate_river_scene(char** scene, const char c, const char side, bool on_boat = false){
  int offset;
  if (on_boat == false){
    offset = (side == 'l')?0:53;
  }
  else{ // on boat
    offset = (side == 'l')?21:38;
  } 

  switch (c){
    case 'M':
      insert_traveller(scene, on_boat?11:2, offset, "missionary.txt");
      break;
    case 'C':
      insert_traveller(scene, 11, offset, "cannibal.txt");
      break;
    case 'B':
      insert_boat(scene, (side == 'l')?0:17);
      break;
    default:
      cerr << "invalid character input" << endl;  
  }
}

// bank input
int read_string(const char* left, int& m, int&c, int& b){
  for (const char* l = left; *l; l++){
    switch (*l){
      case 'M': m++; break;
      case 'C': c++; break;
      case 'B': b++; break;
      case ' ': break;
      default: return ERROR_INPUT_STREAM_ERROR;
    }
  }
  if (m>3){
    return ERROR_INVALID_MISSIONARY_COUNT;
  }
  if (c>3){
    return ERROR_INVALID_CANNIBAL_COUNT;
  }
  return 0;
}

// boat input
int read_string(const char* left, int& m, int&c){
  for (const char* l = left; *l; l++){
    switch (*l){
      case 'M': m++; break;
      case 'C': c++; break;
      case ' ': break;
      default: 
        cerr << "Expected only M and C on the boat" << endl;
        return ERROR_INPUT_STREAM_ERROR;
    }
  }
  if (m>2){
    return ERROR_INVALID_MISSIONARY_COUNT;
  }
  if (c>2){
    return ERROR_INVALID_CANNIBAL_COUNT;
  }
  if (m+c>2 || m+c==0){
    return ERROR_INVALID_MOVE;
  }
  return 0;
}

int form_string(char* str, int m, int c, int b){
  int index = 0;
  if (m>3 || m<0){
    return ERROR_INVALID_MISSIONARY_COUNT;
  }
  if (c>3 || c<0){
    return ERROR_INVALID_CANNIBAL_COUNT;
  }
  for (int index_m = 0; index_m < m; index_m ++){
    str[index] = 'M';
    index++;
  }
  for (int index_c = 0; index_c < c; index_c ++){
    str[index] = 'C';
    index++;
  }
  if (b){
    str[index]='B';
    index++;
  }
  str[index]='\0';
  return 0;
}

// make a river scene based on the characters on the left bank and on the boat
char** make_river_scene(const char* left, const char* boat){

  char** scene = create_scene();
  add_river_base(scene);

  int miss_l = 0, cann_l = 0, boat_l = 0;
  read_string(left, miss_l, cann_l, boat_l);

  int miss_b = 0, cann_b = 0;
  read_string(boat, miss_b, cann_b);

  char right[8];
  int miss_r = 3 - (miss_l + miss_b);
  int cann_r = 3 - (cann_l + cann_b);
  form_string(right, miss_r, cann_r, 1 - boat_l);

  char side = 'l';

  // ADD TO THE LEFT BANK
  for (const char* c = left; *c; c++){
    populate_river_scene(scene, *c, side);
    }

  // ADD THE BOAT IF NOT ADDED BEFORE
  if (!boat_l){
    side = 'r';
    populate_river_scene(scene, 'B', side);
  }

  // loop through the boat characters
  for (const char* c = boat; *c; c++){
    populate_river_scene(scene, *c, side, true);
  }

  // loop through the remaining characters to add them to the right bank
  for (const char* r = right; *r; r++){
    if (*r!='0'){
      populate_river_scene(scene, *r, 'r');
    }
  }

  return scene;
}

int load_boat(char* left, const char* target){
  // remove OR add target to the left bank and place target on the boat

  // read in how many m, c, b are on the left bank
  int miss_l = 0, cann_l = 0, boat_l = 0, result;
  result = read_string(left, miss_l, cann_l, boat_l);

  if (result){
    return result;
  }

  int miss_b = 0, cann_b = 0;

  // read how many m and c are expected to move in this boat
  result = read_string(target, miss_b, cann_b);
  if (result){
    return result;
  }

  // if boat is on the left bank, we have to remove the target pieces from the left bank
  int direction = boat_l ? -1 : 1;
  // rewrite the counts for m and c
  miss_l  = miss_l + miss_b * direction;
  cann_l  = cann_l + cann_b * direction;

  // rewrite the left bank string based on the target
  result = form_string(left, miss_l, cann_l, boat_l);

  if (result){
    return result;
  }

  if (direction==-1 && miss_l<cann_l && miss_l != 0){
    return ERROR_MISSIONARIES_EATEN;
  }
  else if (direction==1 && 3-miss_l<3-cann_l && 3-miss_l!=0){
    return ERROR_MISSIONARIES_EATEN;
  }

  return 0;
}

void crossing_river(char* left, const char* target){
  // move the boat from the left to right
  int miss_l = 0, cann_l = 0, boat_l = 0;
  read_string(left, miss_l, cann_l, boat_l);
  boat_l = boat_l?0:1;
  form_string(left, miss_l, cann_l, boat_l);
}

int unloading_boat(const char* left){
  int miss_l = 0, cann_l = 0, boat_l = 0;
  read_string(left, miss_l, cann_l, boat_l);

  if (boat_l == 0 && ((3-miss_l) < (3-cann_l) && 3-miss_l!=0)){
    return ERROR_MISSIONARIES_EATEN;
  }
  else if (boat_l != 0 && miss_l < cann_l && miss_l != 0){
    return ERROR_MISSIONARIES_EATEN;
  }
  return 0;
}

int perform_crossing(char* left, const char* target){
  int result;

  result = load_boat(left, target);
  if (result){
    return result;
  }

  // create the scene
  cout << "Loading the boat..."<<endl;
  char** scene = make_river_scene(left, target);
  print_scene(scene);

  crossing_river(left, target);
  
  cout << "Crossing the river..."<<endl;
  scene = make_river_scene(left, target);
  print_scene(scene);

  result = unloading_boat(left);
  if (result){
    return result;
  }

  cout << "Unloading the boat..."<<endl;
  scene = make_river_scene(left, "");
  print_scene(scene);
  destroy_scene(scene);

  if (!strcmp(left,"")){
    return VALID_GOAL_STATE;
  }
  else{
    return VALID_NONGOAL_STATE;
  }
}

int play_game(){
  char left[8];
  char boat[3];

  form_string(left,3,3,1);
  char** scene = make_river_scene(left, "");

  cout << "Starting from the initial position..."<<endl;
  print_scene(scene);
  int result = 1;

  while (result==1){
    cout << "Input between 1 and 2 characters to move :"<<endl;
    cin >> boat;
    result = perform_crossing(left, boat);
  }

  return result;
}
