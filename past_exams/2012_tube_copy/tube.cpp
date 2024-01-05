#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

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

/*my functions*/

/* Q1
Write function get symbol position(map,height,width,target,r,c)
that, given a symbol character target finds the coordinates (r,c)
of the first occurrence of that symbol on an ASCII map with
height rows and width columns when scanned in a row-by-row
fashion. If the symbol is found, the function should return true
and set the output parameters r (the row number) and c (the
column number), both of which are indexed starting from 0. If
the symbol cannot be found, the function should return false and
r and c should both be set to -1.
*/
bool get_symbol_position(char** map, int height, int width, const char target, int& r, int& c){
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      if (map[row][col] == target){
        r = row;
        c = col;
        return true;
      }
    }
  }
  r = -1; c = -1;
  return false;
}

/* Q2
Write function get symbol for station or line(name) which,
given the input parameter name describing the name of a station
or line, returns the corresponding map symbol character. If there
is no such station or line, return the space character ’ ’.
*/
//eg: get_symbol_for_station_or_line("Victoria")


char get_symbol_for_station_or_line(const char* name){
  ifstream in;
  int len = 512;
  char line[len];
  if(strcmp((name + strlen(name)-4),"Line")==0){
    in.open("lines.txt");
  }
  else{
    in.open("stations.txt");
  }

  if (!in.good()){
    return ' ';
  }

  in.getline(line, len - 1);
  
  while (!in.eof()){
    if (strcmp((line+2), name) == 0){
      in.close();
      // we found the line/station we were looking for
      return *line;
    }
    in.getline(line, len - 1);
  }
  in.close();
  return ' ';
}

bool get_station_name(const char station_symbol, char* destination){
  if (!isalnum(station_symbol)){
    return false;
  }

  ifstream in;
  char line[512];

  in.open("stations.txt");

  if (!in.good()){
    return false;
  }

  in.getline(line, 512 -1);
  
  while (!in.eof()){
    if (*line == station_symbol){
      in.close();
      // we found the line/station we were looking for
      strcpy(destination, line+2);
      return true;
    }
    in.getline(line, 512 -1);
  }
  in.close();
  return false;
}

//strcpy(route, "S,SE,S,S,E,E,E,E,E,E,E,E,E,E,E");
//int result = validate_route(map, height, width, "Oxford Circus", route, destination);
// Direction string_to_direction(const char *token)

bool not_valid_direction(const char dir){
  if (dir!='N' && dir!='S' && dir!='E' && dir!='W'){
    return true;
  }
  return false;
}

bool get_next_direction(char* direction, char*& route){
    int position = 0;
    while (isalpha(*route)){
      if (not_valid_direction(*route) || position >= 2){
        return false;
      }
      direction[position] = *route;
      position++; route++;
    }

    if (*route!='\0' && *route!=','){
      return false;
    }
    route++;
    direction[position] = '\0';
    return true;
}

// const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
bool make_move(Direction dir, int &row, int& col, int height, int width){
  switch(dir){
    case N: row--; break;
    case S: row++; break;
    case W: col--; break;
    case E: col++; break;
    case NE: row--; col++; break;
    case SE: row++; col++; break;
    case NW: row--; col--; break;
    case SW: row++; col--; break;
    default: return false;
  }
  if (row<0 || col<0 || row>=height || col>=width){
    return false;
  }
  return true;
}

bool opposite_moves(Direction first, Direction second){
  int row = 1, col = 1;
  make_move(first, row, col, 3, 3);
  make_move(second, row, col, 3, 3);
  if (row == 1 && col == 1){
    return true;
  }
  return false;
}

int validate_route(char** map, int height, int width, const char* start_station, char* route, char* destination){
  int row, col, row_before, col_before, count_changes = 0;
  char direction_str[3], previous_line_char;
  Direction direction, direction_before;
  // input parameter start station must be valid
  char start_symbol = get_symbol_for_station_or_line(start_station);
  bool starting_move_from_station = false;

  if (!isalnum(start_symbol)){
    return ERROR_START_STATION_INVALID;
  }
  if (!get_symbol_position(map, height, width, start_symbol, row, col)){
    return ERROR_START_STATION_INVALID;
  }

  while (*route){

    starting_move_from_station = isalnum(map[row][col]);

    if (!get_next_direction(direction_str, route)){
      return ERROR_INVALID_DIRECTION;
    }
    // Keep track of the line before moving when not at a station
    if (!starting_move_from_station){ // if not a station store the previous direction
      direction_before = direction; 
      previous_line_char = map[row][col];
    }

    direction = string_to_direction(direction_str);

    if (direction==INVALID_DIRECTION){
      return ERROR_INVALID_DIRECTION;
    }

    // Store the previous location
    row_before = row; col_before = col;
    // attempt to make the move
    if (!make_move(direction, row, col, height, width)){
      return ERROR_OUT_OF_BOUNDS;
    }
    bool arrived_to_station = isalnum(map[row][col]);
    if (map[row][col]==' '){
      return ERROR_OFF_TRACK;
    }
    
    if (map[row][col]!=map[row_before][col_before])
    {
      if (!starting_move_from_station && !arrived_to_station){
        return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
      }
      else if(!arrived_to_station && previous_line_char != map[row][col]){
        count_changes++;
      }
    }
    // if we have not starting from a station
    if (!starting_move_from_station){
      if (opposite_moves(direction_before, direction)){
        return ERROR_BACKTRACKING_BETWEEN_STATIONS;
      }
    }
  }

  // obtain the station name at the current position
  if (!get_station_name(map[row][col], destination)){
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }
  return count_changes;
}

/*
Determines if the route is valid following rules:
[X] The input parameter start station must be a valid station
name
[X] The input parameter route should be a character string de-
scribing a sequrence of directions (i.e. “N”, “S”, “W”, “E”,
“NE”, “NW”, “SE”, “SW”) separated by commas.
[X] If this route strays outside the bounds of the map,
return ERROR OUT OF BOUNDS. If the route strays off a station
or line/track, return ERROR OFF TRACK.

[X] Line changes can only take place at stations (since train doors
are firmly closed when travelling between stations for safety
reasons).
[X] An attempt to retrace a journey step outside of a station
is not permitted (since trains travel from station to station
without reversing). If an attempt is made to do this, return
ERROR BACKTRACKING BETWEEN STATIONS.
[X] The endpoint of the passenger journey should be a station. If
not, return ERROR ROUTE ENDPOINT IS NOT STATION.
*/

