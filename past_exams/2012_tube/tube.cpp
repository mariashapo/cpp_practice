#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

#include "tube.h"

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

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char** map, int height, int width, const char target, int& r, int& c){
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      if (map[row][col]==target){
        r = row;
        c = col;
        return true;
      }
    }
  }
  r = -1;
  c = -1;
  return false;
}

void filer(char* str){
  while (*str){
    if (isalpha(*str))
      str++;
  else
    *str = '\0';
  }
}

char get_symbol_for_station_or_line(const char* name, const char*filename){
  char line[100];

  ifstream in(filename);
  in.getline(line,100); // is it correct to put a 100 here?
  if (!in.is_open()){
    cerr << "File not opened"<<endl;
    return ' ';
  }

  //filer(line);

  while (!in.eof()){
    if (!strcmp(name,line+2)){
      return line[0];
    }
    in.getline(line,100);
  }

  in.close();

  return ' ';
}

char get_symbol_for_station_or_line(const char* name){
  char ch;
  ch = get_symbol_for_station_or_line(name, "lines.txt");
  if (ch!=' '){
    return ch;
  }
  ch = get_symbol_for_station_or_line(name, "stations.txt");
  if (ch!=' '){
    return ch;
  }
  return ' ';

}

bool get_station_name(const char symbol, const char*filename, char* destination){
  char line[100];

  ifstream in(filename);
  in.getline(line,100); // is it correct to put a 100 here?
  if (!in.is_open()){
    cerr << "File not opened"<<endl;
    return false;
  }

  while (!in.eof()){
    if (symbol==*line){
      strcpy(destination, line + 2);
      return true;
    }
    in.getline(line,100);
  }

  in.close();

  return false;
}
//{N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

Direction get_direction(const char* move){
  if (strlen(move)==1){
    switch (*move){
      case 'N': return N;
      case 'S': return S;
      case 'W': return W;
      case 'E': return E;
      default: return INVALID_DIRECTION;
    }
  }
  if (strlen(move)==2){
    if (*move=='N'){
      if (*(move+1)=='W') return NW;
      else if (*(move+1)=='E') return NE; 
    }
    else if (*move=='S'){
      if (*(move+1)=='W') return SW;
      else if (*(move+1)=='E') return SE; 
    }
  }

  return INVALID_DIRECTION;
}

bool move(int height, int width, int& row, int& col, const Direction& one_move){
    switch (one_move){
      case N: return (--row>=0);
      case S: return (++row<height);
      case W: return (--col>=0);
      case E: return (++col<width);
      case NE: return (--row>=0)&&(++col<width);
      case NW: return (--row>=0)&&(--col>=0);
      case SE: return (++row<height)&&(++col<width);
      case SW: return (++row<height)&&(--col>=0);
      default: return false;
  }
}

bool not_backtracking(int height, int width, int row, int col, int row_before, int col_before, const Direction& one_move){
  move(height, width, row, col,one_move);
  if (row == row_before && col == col_before){
    return false;
  }
  else{
    return true;
  }
}

bool parse_route(const char* route, vector<Direction>& moves){
  char my_char[3];
  int i = 0;
  Direction dir;
  for (const char* move = route; *move; move++){
    if (isalpha(*move)){
      my_char[i]=*move;
      i++;
      if (i>2){
        return false;
      }
    }
    else if (*move == ','){
      my_char[i]='\0';
      i = 0;
      dir = get_direction(my_char);
      if (dir == INVALID_DIRECTION){
        return false;
      }
      moves.push_back(dir);
    }
  }
  moves.push_back(dir);
  return true;
}

int validate_route(char** map, int height, int width, const char* start_station, const char* route, char* destination){
  vector<Direction> moves;
  
  int row, col, line_change = 0;
  int row_prev, col_prev;
  char current_line = '\0';

  char start_symbol = get_symbol_for_station_or_line(start_station, "stations.txt");
  if (start_symbol==' ' ||!get_symbol_position(map, height, width, start_symbol, row, col)){
    return ERROR_START_STATION_INVALID;
  }

  if (!parse_route(route, moves)){
    return INVALID_DIRECTION;
  }

  for (const Direction& one_move: moves){

    // check if not backtracking
    if (!isalnum(map[row][col]) && !isalnum(map[row_prev][col_prev])){
      if (!not_backtracking(height, width, row, col, row_prev, col_prev, one_move)){
        return ERROR_BACKTRACKING_BETWEEN_STATIONS;
      }
    }

    row_prev = row;
    col_prev = col;

    if (!move(height, width, row, col, one_move)){
      return ERROR_OUT_OF_BOUNDS;
    }

    if (map[row][col]==' '){
      return ERROR_OFF_TRACK;
    }

    // check if not hopping between stations
    if (map[row][col]!=map[row_prev][col_prev]){
      if (!(isalnum(map[row][col])||isalnum(map[row_prev][col_prev]))){
        return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
      }
      else if (isalnum(map[row_prev][col_prev]) && !isalnum(map[row][col])){ // was at a station, but on a line
        if (current_line && map[row][col]!=current_line){
          line_change++;
        }
        current_line = map[row][col];
      }
    }
  }

  char end_station = map[row][col];

  if (!isalnum(end_station)){
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }

  if (!get_station_name(end_station, "stations.txt", destination)){
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }

  return line_change;
}