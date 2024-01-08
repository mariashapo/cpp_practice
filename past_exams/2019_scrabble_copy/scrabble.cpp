#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */


int tile_score(const char letter){
    if (letter == ' ' || letter == '?'){
        return 0;
    }
    if (!isalpha(letter)){
        return - 1;
    }
    switch(toupper(letter)){
        case 'Z': case 'Q': return 10;
        case 'X': case 'J': return 8;
        case 'K': return 5;
        case 'H': case 'F': case 'V': case 'W': case 'Y': return 4;
        case 'B': case 'C': case 'M': case 'P': return 3;
        case 'D': case 'G': return 2;
        default: return 1;
    }
}

/*
Write a recursive Boolean function can_form_word_from_tiles(word, tiles, played_tiles) which
determines whether a given word can be made from a given collection of tiles. Here word is an input
2 type describing the target word while tiles is an input parameter of string type
parameter of string
describing the tiles in the collection. You may assume that the target word is a valid English word. If
the target word can be formed from the tiles according to the rules described in the Problem Description,
the function should return true, and output parameter played_tiles should be a null-terminated string
consisting of the ordered collection of tiles used. Otherwise the function should return false.
*/

// bool success = can_form_word_from_tiles("LEAP", "ZLEAP?S", played_tiles)
bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles){
    if (*word == '\0'){
        *played_tiles = '\0';
        return true;
    }
    char tiles_copy[strlen(tiles) + 1];
    strcpy(tiles_copy, tiles);

    char* find_letter = strchr(tiles_copy, *word);
    char* find_wild = strchr(tiles_copy, '?');

    if (find_letter != NULL){
        *played_tiles = *word; // add the tile to the list of played tiles
        *find_letter = '#'; // mask the used letter
        return can_form_word_from_tiles(word+1, tiles_copy, played_tiles + 1);
    }
    else if(find_wild != NULL){
        *played_tiles = '?';
        *find_wild = '#';
        return can_form_word_from_tiles(word+1, tiles_copy, played_tiles + 1);
    }

    return false;
}

/*
Write an integer-valued function compute_score(played_tiles,
score_modifiers) which returns the
word score given a played_tiles string and an array of score_modifiers. The individual elements in
the score_modifiers array are of enumerated type ScoreModifier.
*/

// ScoreModifier sm0[]={NONE, TRIPLE_LETTER_SCORE, NONE, NONE, DOUBLE_WORD_SCORE, NONE, NONE};

int compute_score(const char* played_tiles, ScoreModifier score_modifiers[]){
    int score = 0;
    int multiplier = 1;
    for (int i = 0; played_tiles[i]; i++){
        switch(score_modifiers[i]){
            case TRIPLE_LETTER_SCORE: score += 3*tile_score(played_tiles[i]); break;
            case DOUBLE_LETTER_SCORE: score += 2*tile_score(played_tiles[i]); break;
            case NONE: score += tile_score(played_tiles[i]); break;
            case TRIPLE_WORD_SCORE: score += tile_score(played_tiles[i]); multiplier *= 3; break;
            case DOUBLE_WORD_SCORE: score += tile_score(played_tiles[i]); multiplier *= 2; break;
        }
    }
    score = score*multiplier;
    if (strlen(played_tiles)==7){
        score+=50;    
    }
    return score;
}

/*
* Write an integer-valued function highest_scoring_word_from_tiles(tiles,
score_modifiers, word) which returns the highest word score that can be achieved 
* given a particular collection of tiles and 
* score modifiers, 
* using any of the words in the supplied dictionary. 
Here tiles and score_modifiers are input
parameters of type string and ScoreModifier[] respectively.
If it not possible to make any word in the supplied dictionary from the tiles then the function should
return -1. Otherwise output parameter word should contain the word attaining the highest word score,
and the function should return the achieved word score.
*/


int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier score_modifiers[], char* word){
    char played_tiles[8], line[100];
    word[0] = '\0';
    int max_score = -1, current_score;
    ifstream in("words.txt");
    if (!in.is_open()){
        return -1;
    }
    in >> line;
    while(!in.eof()){
        
        if (can_form_word_from_tiles(line, tiles, played_tiles)){
            // compute the score
            current_score = compute_score(played_tiles, score_modifiers);
            if (current_score > max_score){
                max_score = current_score;
                strcpy(word, line);
            }
        }
        in >> line;
        played_tiles[0]='\0'; // reset the played tiles
    }
    
    in.close();
    return max_score;
}