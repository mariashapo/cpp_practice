#ifndef SPELL_H
#define SPELL_H
/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512

/* insert your function prototypes here */

// Q1
int frequency(const char*);

//Q2
int edit_distance(const char*, const char*);

//Q3
bool spell_correct(const char* word, char* fixed);
int edit_distance_bonus(const char* a, const char* b);

#endif