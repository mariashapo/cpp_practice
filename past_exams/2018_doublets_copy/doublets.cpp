#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

/*Write a Boolean function valid step(current word, next word) which returns true if the step from
current word to next word represents a valid step in a Doublet chain according to Rule 2 and Rule 4 of
Figure 2. You may assume that both words are supplied in uppercase format4 .*/

bool valid_step(const char* current_word, const char* next_word){
   int count = 0;
   if (strlen(current_word)!=strlen(next_word)){
      return false;
   }
   for (int i = 0; current_word[i]; i++){
      if (current_word[i]!=next_word[i]){
         count++;
      }
   }

   if (!dictionary_search(next_word) || count!=1){
      return false;
   }
   else{
      return true;
   }
}


/*Write a Boolean function display chain(chain, output stream) which writes a given chain to an
output stream according to the conventions for the presentation of chains described in the Problem
Description. The input parameter chain is a NULL-terminated array of uppercase C-style strings
representing the words found at each step of the chain, while the input parameter output stream can
be any valid output stream, including cout. The function should return true if the entire chain was
successfully written to the output stream, and false otherwise. For example, the code:*/

// eg: const char *tea_chain[] = {"TEA", "SEA", "SET", "SOT", "HOT", NULL };


bool display_chain(const char** chain, ostream& output){
   if (!*chain || !output.good()){
      return false;
   }
   output << *chain << endl;
   chain++;
   if (!*chain){
      return false;
   }
   for (; *(chain+1); chain++){
      for (const char* letter = *chain; *letter; letter++){
         output << static_cast<char>(tolower(*letter));
      }
      output << endl;
   }
   output << *chain << endl;
   return output.good();
}

/*Write a Boolean function valid chain(chain) which returns true if and only if the given chain is a
valid Doublets chain according to all four rules of Figure 2. As before, the input parameter chain is a
NULL-terminated array of uppercase C-style strings representing the words found at each step of the
chain.*/

bool valid_chain(const char** wheat_chain){
   if (!*wheat_chain || !*(wheat_chain + 1)){
      return false;
   }
   for (; *(wheat_chain + 1); wheat_chain++){
      if (!valid_step(*wheat_chain, *(wheat_chain+1))){
         return false;
      }
   }
   return true;
}

/*Write a Boolean function find chain(start word, target word, answer chain, max steps) which
attempts to find a valid chain beginning with start word and ending with target word in up to
max steps steps.
If a valid chain can be found, output parameter answer chain should contain the
found chain (in the form of a NULL-terminated array of uppercase C-style strings) and the function
should return true. Otherwise the function should return false.*/

int num_wrong_letters(const char* start_word, const char* target_word){
   int count = 0;
   for (int i = 0; start_word[i]; i++){
      if (start_word[i] != target_word[i]){
         count++;
      }
   }
   return count;
}

bool not_already_present(const char* current_word, const char** answer_chain){
   for (int i = 0; answer_chain[i]; i++){
      if (strcmp(answer_chain[i], current_word) == 0){
         return false;
      }
   }
   return true;
}

int get_null_index(const char** answer_chain){
   int i = 0;
   while (answer_chain[i]){
      i++;
   }
   return i;
}

const char* get_last_word(const char** answer_chain){
   int i = get_null_index(answer_chain);
   return answer_chain[i - 1];
}

void add_last(const char** answer_chain, const char* word){
   int i = get_null_index(answer_chain);
   
   // allocate memory
   //answer_chain[i] = new char[strlen(word) + 1];
   answer_chain[i] = word;
   answer_chain[i+1] = nullptr;
}

int num_steps(const char** answer_chain){
   int count = 0;
   for (int i = 1; answer_chain[i]; i++){
      count ++;
   }
   return count;
}

void delete_last(const char** answer_chain){
   int i = get_null_index(answer_chain);
   //delete [] answer_chain[i - 1];
   answer_chain[i-1] = nullptr;
   }

bool find_chain_recursive(const char* target_word, const char** answer_chain, int steps){
   //cout << num_steps(answer_chain) << endl;
   int word_len = strlen(target_word);
   const char* current_word = get_last_word(answer_chain);

   // termination success
   if (!strcmp(current_word, target_word) && valid_chain(answer_chain)){
      return true;
   }

   if (steps <= 0 || num_wrong_letters(current_word, target_word) > steps){ //|| max_steps < num_wrong_letters(current_word,target_word)
      return false;
   }

   char original_char;
   // allocate memory for the next word
   char* next_word = new char [word_len + 1];
   strcpy(next_word, current_word);
   //cout << next_word << endl;

   for (int index = 0; index < word_len; index++){
      original_char = next_word[index];
      
      for (char new_letter = 'A'; new_letter <= 'Z'; new_letter++){
         if (new_letter == original_char){continue;}
         
         next_word[index] = new_letter;

         if (!dictionary_search(next_word)){continue;}

         add_last(answer_chain, next_word);
         if (find_chain_recursive(target_word, answer_chain, steps - 1)){
            return true;
         }
         delete_last(answer_chain);
      }
      next_word[index] = original_char;
   }
   // only need to delete when existing the current iteration
   delete [] next_word;
   return false;
}

bool find_chain(const char* start_word, const char* target_word, const char** answer_chain, int max_steps){
   int num_step = 0;
   answer_chain[num_step++] = start_word;
   answer_chain[num_step] = nullptr;
   
   if (strlen(start_word) != strlen(target_word)){
      return false;
   }
   bool result = find_chain_recursive(target_word, answer_chain, max_steps);
   return result;
}
