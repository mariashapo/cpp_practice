#include <cstring>
#include <cctype>
#include <iostream>
#include "piglatin.h"

using namespace std;

bool isVowel(const char ch, int position, int length){
    for (const char* vow = VOWELS; *vow; vow++){
        if (tolower(ch) == *vow){
            return true;
        }
        else if (tolower(ch)=='y'&& position!=0 && position < length){
            return true;
        }
    }
    return false;
}

int findFirstVowel(const char* str){
    int length = strlen(str);
    const char* s;
    for (int position = 0; position < length; position++){
        s = str + position;
        if (isVowel(*s, position, length)){
            return position;
        }
    }
    return -1;
}

void clear_str(char* translated){
    for (char* t = translated; *t; t++){
        *t = '\0';
    }
}

void translateWord(const char* english, char* translated){
    int first_vow;
    int length = strlen(english);
    clear_str(translated);
    first_vow = findFirstVowel(english);
    if (first_vow == 0){
        strcpy(translated,english);
        // add "way" after english
        strcpy(translated+length,"way");
    }
    else if (strcmp(english,"")==0 || length == 0){
        translated[0] ='\0';
    }
    else if (isdigit(english[0])){
        strcpy(translated,english);
    }
    else if (first_vow == -1 && isalpha(english[0])){
        strcpy(translated,english);
        strcpy(translated+length,"ay"); 
    }
    else if (length > 0 && first_vow >0){
        int len_to_add = length - first_vow;
        strncpy(translated, english + first_vow, len_to_add);
        strncat(translated, english, first_vow);
        strcat(translated, "ay");
        if (isupper(english[0])){
            translated[0] = toupper(translated[0]);
            translated[len_to_add] = tolower(translated[len_to_add]);
        }
    }
    else{
        translated[0] ='\0';
    }
}


char punc_1(char* english){

 if (ispunct(english[0])){
    char punct = *english;
    for (char* e = english; *(e+1); e++){
        *e = *(e+1);
    }
    english[strlen(english)-1] = '\0';
    return punct;
 }
 return '\0';
}

char punc_2(char* english){

 if (ispunct(english[strlen(english)-1])){
    char punct = english[strlen(english)-1];
    english[strlen(english)-1] = '\0';
    return punct;
 }
 return '\0';
}

void translateStream(ifstream& input, ostream& output){
    if (input.eof()){
        cout << endl;
        return;
    }
    char my_char;
    char english[64];
    char translated[64];

    if (!isalpha(next) && !isdigit(next)) {
        output << next;
        translateStream(input, output);
        return;
    }

    while (isalpha(next) || isdigit(next) || next == '-') {
        word[index] = next;
        index++;
        input.get(next);
    }
    word[index] = '\0';

    translateWord(word, translatedWord);
    output << translatedWord << next;
    translateStream(input, output);
}
