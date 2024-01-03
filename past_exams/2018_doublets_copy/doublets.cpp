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

bool valid_step(const char* current_word, const char* next_word){
   int count = 0;

   // check that same length
   if (strlen(next_word)!=strlen(current_word)){return false;}
   int len = strlen(current_word);

   // check that only one word differs
   for (int index = 0; index < len; index++){
      if (current_word[index] != next_word[index]){
         count++;
      }
   }

   // check that the next_word is in the dictionary
   if (!dictionary_search(next_word)){return false;}

   if (count == 1){return true;}
   else {return false;}
}


/*Write a Boolean function display chain(chain, output stream) which writes a given chain to an
output stream according to the conventions for the presentation of chains described in the Problem
Description. The input parameter chain is a NULL-terminated array of uppercase C-style strings
representing the words found at each step of the chain, while the input parameter output stream can
be any valid output stream, including cout. The function should return true if the entire chain was
successfully written to the output stream, and false otherwise. For example, the code:*/

bool display_chain(const char** chain, ofstream& out){
   for (const char** word = chain; *word; word++){
      if (word==chain || !*(word+1)){
         out<<*word;
      }
      else {
         for (const char* letter = *word; *letter; letter++){
            out<<static_cast<char>(tolower(*letter));
         }
      }
      out<<'\n';
   }
   return out.good();
}

bool display_chain(const char** chain, ostream& out){
   for (const char** word = chain; *word; word++){
      if (word==chain || !*(word+1)){
         out<<*word;
      }
      else {
         for (const char* letter = *word; *letter; letter++){
            out<<static_cast<char>(tolower(*letter));
         }
      }
      out<<'\n';
   }
   return out.good();
}

/*Write a Boolean function valid chain(chain) which returns true if and only if the given chain is a
valid Doublets chain according to all four rules of Figure 2. As before, the input parameter chain is a
NULL-terminated array of uppercase C-style strings representing the words found at each step of the
chain.*/

bool valid_chain(const char** chain){
   for (const char** word = chain; *(word+1); word++){
      if (!valid_step(*word, *(word+1))){
         //cout << *(word) << endl;
         //cout << *(word + 1)<< endl;
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

bool find_chain(const char* start_word, const char* target_word, const char** answer_chain, int max_steps, char* next_word, int position){
   if(!next_word){ // first iterration
      next_word = new char[strlen(start_word)+1];
      strcpy(next_word, start_word);
   }
   cout << next_word << " ";

   // copy the next word into the current word, so that comparisons can be made
   char current_word[strlen(start_word)+1];
   strcpy(current_word, next_word);

   // solution found
   if (strcmp(target_word, current_word)==0){
      return true;
   }
   if (max_steps == 0){
      return false;
   }
   cout << position << " ";
   position = position % strlen(start_word);
   
   // loop through all the possible transformations for the given index
   for (char c = 'A'; c<='Z'; c++){
      next_word[position]=c;
      if (valid_step(current_word, next_word)){
         if (find_chain(start_word, target_word, answer_chain, max_steps-1, next_word, position+1)){
            cout << next_word << " ";
            return true;
         }
      }
      // we have to be able to skip letters
      else if(current_word[position]==c){
            if (find_chain(start_word, target_word, answer_chain, max_steps, next_word, position+1)){
            cout << next_word << " ";
            return true;
         }

      }
      next_word[position]=current_word[position];
   }

   return false;
}
