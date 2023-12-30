#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

int tile_score(const char tile){
    if (!(isalpha(tile) || tile=='?'|| tile==' ')){
        return -1;
    }
    switch (tolower(tile)){
        case ' ' : case '?' : return 0;
        case 'q' : case 'z': return 10;
        case 'x' : case 'j': return 8;
        case 'k' : return 5;
        case 'h' : case 'v': case 'w': case 'y': case 'f': return 4;
        case 'b' : case 'c': case 'm': case 'p': return 3;
        case 'd' : case 'g': return 2;
        default: return 1;
    }
}

char use_tile(const char letter, char* tiles){
    char used_tile;
    for (char* tile = tiles; *tile; tile++){
        if (*tile == letter){
            *tile = '.';
            return letter;
        }
    }
    for (char* tile = tiles; *tile; tile++){
        if (*tile == ' ' || *tile =='?'){
            used_tile = *tile;
            *tile = '.';
            return used_tile;
        }
    }
    return '-';
}

bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles, char* current_tiles){
    char used_tile;

    if (!current_tiles){
        current_tiles = new char[8];
        strcpy(current_tiles, tiles);
    }
    if (*word=='\0'){
        *played_tiles = '\0';
        delete [] current_tiles;
        return true;
    }

    used_tile = use_tile(*word, current_tiles);
    if (used_tile!='-'){
        *played_tiles = used_tile;
    }
    else{
        return false;
    }

    return can_form_word_from_tiles(word+1, current_tiles, played_tiles+1);    
}


int letter_modifier(const ScoreModifier& score_modifier){
    switch(score_modifier){
        case DOUBLE_LETTER_SCORE: return 2;
        case TRIPLE_LETTER_SCORE: return 3;
        default: return 1;
    }
}

int word_modifier(const ScoreModifier& score_modifier){
    switch(score_modifier){
        case DOUBLE_WORD_SCORE: return 2;
        case TRIPLE_WORD_SCORE: return 3;
        default: return 1;
    }
}

// {NONE, TRIPLE_LETTER_SCORE, NONE, NONE, DOUBLE_WORD_SCORE, NONE, NONE};

int compute_score(const char* played_tiles, const ScoreModifier* score_modifiers){
    int score = 0, score_mult = 1;
    for (const char* tile = played_tiles; *tile; tile++, score_modifiers++){
        score += tile_score(*tile)*letter_modifier(*score_modifiers);
        score_mult*=word_modifier(*score_modifiers);
    }

    score*=score_mult;
    if (strlen(played_tiles)==7){
        score+=50;
    }
    return score;
}

//   score = highest_scoring_word_from_tiles("WBNNOER", sm1, word);
int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers, char* word){
    ifstream in("words.txt");
    word[0]='\0';
    int highest_score = 0;
    char line[512], played_tiles[80];

    if (!in.is_open()){
        cerr << "file not opened" << endl;
        return -1;
    }

    in >> line;
    while (!in.eof()){
        if (can_form_word_from_tiles(line, tiles, played_tiles)){
            int current_score = compute_score(played_tiles, score_modifiers);
            if (current_score>highest_score){
                highest_score = current_score;
                strcpy(word, line);
            }
        }
        in >> line;
    }
    if (*word){
        return highest_score;
    }
    else{
        return -1;
    }
}
