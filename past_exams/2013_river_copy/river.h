#ifndef RIVER_H
#define RIVER_H

const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};

char **allocate_2D_array(int rows, int columns);

void deallocate_2D_array(char **m, int rows);

char **create_scene();

void destroy_scene(char **scene);

void print_scene(char **scene);

void filter(char *line);

bool add_to_scene(char **scene, int row, int col, const char *filename);

const char *status_description(int code);

/* my functions */

/*Write a function make river scene(left, boat) which takes
two character strings: left describing the contents of the left
river bank, and boat describing the contents of the boat2, and
which returns a corresponding ASCII-art scene. */
char** make_river_scene(const char* left, const char* boat);

/*Write a function perform crossing(left, targets) which per-
forms a crossing of the river by one or two target entities4 from one
bank of the river to the other using the boat. */
int perform_crossing(char* left, const char* targets, bool print = true);

/*Write a function play game() which allows a user to play the
game of missionaries and cannibals by suggesting boat crossings
via the keyboard. The return value of the function should be
an appropriate status code e.g. ERROR MISSIONARIES EATEN or
VALID GOAL STATE (i.e. everyone crossed successfully).*/
int play_game();

int find_solution(char* left, char* answer, int move_count = 0);
#endif