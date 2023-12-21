#include <cctype>
#include "soundex.h"

void encode(const char* surname, char* soundex){
    soundex[0]=toupper(surname[0]);
    int index = 1;
    for (const char* c = surname +1; *c; c++){
        char* ch = map(*c);
        if (ch!=nullptr && *ch!=soundex[index-1]){
            soundex[index]=*ch;
            index++;
            delete ch;
            if (index==4){
                break;
            }
        }
    }
    while (index!=4){
        soundex[index]='0';
        index++;
    }
    soundex[index]='\0';
}

char* map(const char ch){
    switch (ch){
        case 'b':
        case 'f':
        case 'p':
        case 'v':
            return new char('1');
        case 'c':
        case 'g':
        case 'j':
        case 'k':
        case 'q':
        case 's':
        case 'x':
        case 'z':
            return new char('2');
        case 'd':
        case 't':
            return new char('3');
        case 'l':
            return new char('4');
        case 'm':
        case 'n':
            return new char('5');
        case 'r':
            return new char('6');
        default:
            return nullptr;
    }
}

bool compare(const char* s1, const char* s2){
    if (*s1=='\0' && *s2=='\0'){
        return true;
    }
    else if (*s1==*s2){
        return (compare(s1+1, s2+1)); 
    }
    else{
        return false;
    }
}

bool compare_words(const char* w1, const char* w2){
    char w1_soundex[5];
    char w2_soundex[5];

    encode(w1, w1_soundex);
    encode(w2, w2_soundex);

    //cout << "<"<<w1_soundex<<"> and <"<< w2_soundex <<">"<< endl;

    return compare(w1_soundex, w2_soundex);
}


int count(const char* surname, const char* sentence){
    int index = 0;
    int count = 0;

    // declare a char array of the max length of the whole sentence
    char sentence_word[strlen(sentence)+1];

    while (*sentence!='.'){
        // get to the first uppercase letter
        while (!isupper(*sentence)){
            sentence++;
        }
        index =0;
        while (isalpha(*sentence)){
            sentence_word[index]=*sentence;
            index++;
            sentence++;
        }
        sentence_word[index]='\0';
        count += compare_words(surname, sentence_word);
    }
    return count;
}

