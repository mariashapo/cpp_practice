#include <iostream>
#include <cctype>
#include <cstring>
#include <fstream>
#include "piglatin.h"

/* QUESTION 1 */

bool isVowel(char letter, int position, int length) {
    if (letter == 'a' ||  letter == 'A') {
        return true;
    }
    if (letter == 'e' ||  letter == 'E') {
        return true;
    }
    if (letter == 'i' ||  letter == 'I') {
        return true;
    }
    if (letter == 'o' ||  letter == 'O') {
        return true;
    }
    if (letter == 'u' ||  letter == 'U') {
        return true;
    }
    if (letter == 'y' && position != length - 1 && position != 1) {
        return true;
    }
    
    return false;
}  

int findFirstVowel(const char* word) {
int length = strlen(word);
int position = 1;

while (*word != '\0') {
    if (isVowel(*word, position, length)) {
        return position;
    }
    position++;
    word++;
}

return -1;
}


/* QUESTION 2 */

void translateWord(const char* english, char piglatin []) {
    int positionfirstVowel = findFirstVowel(english);
    bool firstLetterCapital = false;
    char lettersToMove[100];
    int index = 0;
    
    if (isupper(*english)) {
        firstLetterCapital = true;
    }
    
    if (isdigit(*english)) {
        strcpy(piglatin, english);
    }
    else if (positionfirstVowel == -1) {
        strcpy(piglatin, english);
        strcat(piglatin, "ay");
    }
    else if (positionfirstVowel == 1) {
        strcpy(piglatin, english);
        strcat(piglatin, "way");
    }
    else {
        for (index = 0; index < positionfirstVowel - 1; index++) {
            lettersToMove[index] = *english;
            english++;
        }
        lettersToMove[index] = '\0';
        if (isupper(lettersToMove[0])) {
            lettersToMove[0] = tolower(lettersToMove[0]);
        }
        strcpy(piglatin, english);
        strcat(piglatin, lettersToMove);
        strcat(piglatin, "ay");
    }

    if (firstLetterCapital == true) {
        piglatin[0] = toupper(piglatin[0]);
    }
}

/* QUESTION 3 */

void translateStream(std::istream& input, std::ostream& output) {
    const int MAX_NUM_CHAR = 64;
    char word[MAX_NUM_CHAR + 1];
     char translatedWord[MAX_NUM_CHAR + 1];
     char next;
     int index = 0;
    
    input.get(next);
    if (input.eof()) {
        return;
    }
    
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
