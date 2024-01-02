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
}
*/
bool already_exists(const char *ending, char** lookup, int& loc){
  for (loc = 0; lookup[loc]; loc++){
    if (strcmp(lookup[loc],ending) == 0){
      //cout << "<" << loc << ">" << endl;
      return true;
    }
  }
  //cout << "<" << loc << ">" << endl;
  return false;
}

int last_index(const char** lookup){
  int i = 0;
  while (lookup[i]){
    i++;
  }
  return 'a' + i;
}

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static int offset = 0; // max offset is 25
  // the table which maps endings to letters (persists between calls)
  static char* lookup [26];

  // providing a way to reset the table between poems
  if (ending == RESET) {
    for (int i = 0; i < offset; i++){
      delete [] lookup[i];
      lookup[i] = nullptr;
    }
    offset = 0;
    return '\0';
  }
  
  //string end(ending);
  int position;
  // if the ending doesn't exist, add it, and issue a new letter
  if (!already_exists(ending, lookup, position)) {
    lookup[position] = new char[100];
    strcpy(lookup[position], ending);

    offset++;
    assert(offset <= 25);
    return 'a' + position;
  }

  // otherwise return the letter corresponding to the existing ending
  return 'a' + position;
}

/* START WRITING YOUR FUNCTION BODIES HERE */


int count_words(const char* line){
   int count = 0;
   bool in_between_words = true;

   for (const char* c = line; *c; c++){
      if (isalpha(*c)){
        in_between_words = false;
      }
      
      else if (*c == ' ' && !in_between_words){
         count++;
         in_between_words = true;
      }
   }

   if (!in_between_words) {++count;}
   return count;
}

bool is_vowel_present(const char* word){
  for (const char* w = word; isalpha(*w); w++){
    if (*w == 'A' || *w == 'E' || *w == 'I' || *w == 'O' || *w == 'U'){
      return true;
    }
  }
  return false;
}

void find_ending(const char* line, char* phonetics){
  for (int i = 0; i<strlen(phonetics); i++){
    phonetics[i]='\0';
  }
  int word_num = count_words(line);

  for (int loc = word_num; loc>1; loc--){
    char word_phonetic[100];
    get_word(line, loc, word_phonetic);
    if (is_vowel_present(word_phonetic)){
      // if there is a vowel, we need to concat all of the leading words
      for (int loc_start = loc; loc_start <=word_num; loc_start++){
        get_word(line, loc_start, word_phonetic);
        strcat(phonetics, word_phonetic);
      }
      return;
    }
  }
}

void to_upper(char* word){
  while (*word){
    *word = toupper(*word);
    word++;
  }
}

void filter(char* word){
  while((isalpha(*word))){
    word++;
  }
  *word='\0';
}

bool find_phonetic_ending(const char* word, char* phonetics){
  char line[512], word_upper[strlen(word)+1];
  strcpy(word_upper, word);
  to_upper(word_upper);

  ifstream in;
  in.open("dictionary.txt");
  if (!in.is_open()){
    return false;
  }

  in.getline(line, 512);

  while(!in.eof()){
    char first_word[100];
    get_word(line, 1, first_word);
    if (strcmp(first_word, word_upper)==0){
      //cout << line << endl;
      find_ending(line, phonetics);
      return true;
    }
    in.getline(line, 512);
  }

  return false;
}

bool find_rhyme_scheme(const char* filename, char* scheme){
  char line[512];
  int word_num;

  scheme[0]='\0';
  ifstream in(filename);

  if (!in.is_open()){
    return false;
  }

  rhyming_letter(RESET);

  in.getline(line, 512);

  int i = 0;
  while(!in.eof()){
    char last_word[100], ending[100];

    //cout << line << endl;

    word_num = count_words(line);
    get_word(line, word_num, last_word);
    find_phonetic_ending(last_word, ending);

    // form the rhyming pattern
    char letter = rhyming_letter(ending);
    scheme[i++] = letter;

    in.getline(line, 512);
  }

  scheme[i] = '\0';

  return true;
}

string identify_sonnet(const char* filename){
  char scheme[512];
  string name;

  find_rhyme_scheme(filename, scheme);

  if (strcmp(scheme, "ababcdcdefefgg")==0){
    name = "Shakespearean";
  }
  else if (strcmp(scheme, "abbaabbacdcdcd")==0){
    name =  "Petrarchan";
  }
  else if (strcmp(scheme, "ababbcbccdcdee")==0){
    name = "Spenserian";
  }
  else{
    name = "Unknown";
  }
  return name;
}

