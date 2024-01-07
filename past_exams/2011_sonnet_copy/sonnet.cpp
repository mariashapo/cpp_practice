#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

/*
char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}*/

void empty_lookup(char** lookup_endings){
  for (int i = 0; lookup_endings[i]; i++){
    delete [] lookup_endings[i];
    lookup_endings[i] = nullptr; 
  }
}

int position(const char* ending, char** lookup_endings){
  int i;
  for (i = 0; lookup_endings[i]; i++){
    if (strcmp(lookup_endings[i], ending)==0){
      return i;
    }
  }
  lookup_endings[i] = new char[100];
  strcpy(lookup_endings[i], ending);
  return i;
}

// bonus
char rhyming_letter(const char *ending) {

  // the table which maps endings to letters (persists between calls)
  static char* lookup_endings[30]; //max 26 letters for the rhyme

  // providing a way to reset the table between poems
  if (ending == RESET) {
    empty_lookup(lookup_endings);
    return '\0';
  }

  // look for the position in the look up table
  int pos = position(ending, lookup_endings);
  if (pos>=26){
    cerr << "too many rhyme letters have been requested, going beyond 'z";
    return '.';
  }
  return 'a' + pos;
  }


/* START WRITING YOUR FUNCTION BODIES HERE */

// Q1
/*
Write a function count words(line) which returns the number
of words in a given input string line.
*/

int count_words(const char* line){
  int word_count = 0;
  bool alpha_present = false;
  while (*line){
    // loop until the end of the line or until a white space
    while (*line && *line != ' '){
      if (isalpha(*line) && !alpha_present){
        // record that a word was encountered in the loop
        alpha_present = true;
      }
      line++;
    }
    // if the end of the line, break
    if (!*line){
      // if there was a word before the break, incement
      if (alpha_present){
        word_count++; 
        return word_count;
      }
    }
    else if (*line == ' ' && alpha_present){
      word_count++;
      alpha_present = false; //reset
    }

    while (*line==' '){
      // reach the next character that is not white space
      line++;
    }

  }

  return word_count;
}

//Q2
/*
Write a function find phonetic ending(word, phonetic ending)
which uses the phonetic dictionary in the file dictionary.txt to
construct the phonetic ending for the (uppercase) word contained
in the input parameter word. If this word is in the phonetic dic-
tionary, the corresponding phonetic ending should be stored in
the output parameter phonetic ending, and the function should
return true. Otherwise the function should return false.

char ending[512];
bool success = find_phonetic_ending("CONVICT", ending);
*/

// a, e, i, o or u.
bool contains_vowel(const char* word){
  for (const char* l = word; *l; l++){
    if (*l == 'A' || *l == 'E' || *l == 'I' || *l == 'O' || *l == 'U'){
      return true;
    }
  }
  return false;
}

void form_ending(const char* line, int position, int word_num, char* ending){
  char word_part[512];
  ending[0] = '\0';
  for (; position<=word_num; position++){
    int len_before = strlen(ending);
    get_word(line, position, word_part);
    strcat(ending, word_part);
    ending[len_before + strlen(word_part)] = '\0';
  }
}

bool get_ending_from_line(char* line, char* ending){
  char word_part[512];
  int word_num = count_words(line);

  for (int position = word_num; position > 1; position--){
    // starting from the last word
    get_word(line, position, word_part);
    if (contains_vowel(word_part)){
      form_ending(line, position, word_num, ending);
      return true;
    }
  }
  return false;
}

bool find_phonetic_ending(const char* word, char* ending){
  char line[512];
  char word_found[512];
  // open the file
  ifstream in("dictionary.txt");

  if (!in.good()){
    return false;
  }

  in.getline(line, 511);

  while(!in.eof()){
    get_word(line, 1, word_found);

    if (strcmp(word_found, word) == 0){
      // the word was found
      get_ending_from_line(line, ending);
      in.close();
    }

    // keep getting the next line
    in.getline(line, 511);
  }

  in.close();
  return false;
}

/*Write a function find rhyme scheme(filename, scheme) which
produces in the output parameter scheme the rhyme scheme for
the sonnet contained in the file filename. If the file does not exist,
the function should return false; otherwise the function should
return true*/

void find_line_ending(const char* line, char* ending){
  char last_word[100];
  int word_count = count_words(line);
  get_word(line, word_count, last_word);
  find_phonetic_ending(last_word, ending);
}

bool find_rhyme_scheme(const char* filename, char* scheme){
  ifstream in(filename);
  char line[512], ending[100];
  int index = 0;

  if (!in.is_open()){
    return false;
  }
  rhyming_letter(RESET);

  in.getline(line, 511);

  while(!in.eof()){
    find_line_ending(line, ending);
    scheme[index++] = rhyming_letter(ending);
    in.getline(line, 511);
  }

  scheme[index] = '\0';

  return true;
}

/*
Write a function identify sonnet(filename) which has as its
return value one of the strings: "Shakespearean", "Petrarchan",
"Spenserian", or "Unknown" according to whether the rhyme
scheme of the sonnet in file filename matches that of a Shake-
spearean, Petrarchan or Spenserian sonnet; if no match can be
found then the string "Unknown" should be returned.
*/

const char* identify_sonnet(const char* filename){
  char scheme[100];
  char* return_str = new char[100];

  if (!find_rhyme_scheme(filename, scheme)){
    strcpy(return_str, "no file");
    return return_str;
  }
  // A Shakespearean sonnet has rhyme scheme ababcdcdefefgg
  // a Petrarchan sonnet has rhyme scheme abbaabbacdcdcd 
  // a Spenserian sonnet has rhyme scheme ababbcbccdcdee
  if (strcmp(scheme, "ababcdcdefefgg") == 0){
    strcpy(return_str, "Shakespearean");
    return return_str;
  }
  else if (strcmp(scheme, "abbaabbacdcdcd") == 0){
    strcpy(return_str, "Petrarchan");
    return return_str;
  }
  else if (strcmp(scheme, "ababbcbccdcdee") == 0){
    strcpy(return_str, "Spenserian");
    return return_str;
  }
  else{
    strcpy(return_str, "Unknown");
    return return_str;
  }
}