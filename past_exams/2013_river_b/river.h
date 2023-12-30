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

#endif