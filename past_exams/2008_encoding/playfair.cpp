#include <cctype>
#include "playfair.h"
using namespace std;

void prepare(const char* input, char* output){
    int index = 0;
    for (const char *c = input; *c ; c++){ // moving through the string
        if (isalnum(*c)){
            output[index]=toupper(*c);
            index++;
        }
    }
    if (index%2==1){
        output[index]='X';
        index++;
    }
    output[index]='\0';
}

void grid(const char* codeword, char square[][6]){
    int index = 0;
    // insert the codeword
    for (const char *c = codeword ; *c ; c++){
        insert(*c, square, index);
    }
    // insert of the alphabetic letters
    for (char letter = 'A'; letter<='Z'; letter++){
        insert(letter, square, index);
    }
    // inser the digits
    for (char digit = '0'; digit <='9'; digit++){
        insert(digit, square, index);
    }
}

void insert(const char character, char square[][6], int& index){
    if (isNew(character, square, index)){
        square[index/6][index%6]=character;
        index++;
    }   
}

// check if a character is new
bool isNew(const char character, const char square[][6], int index){
    for (int i = 0; i<index; i++){
        if (square[i/6][i%6]==character){
            return false;
        }
    }
    return true;
}

void bigram(const char square[][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2){
    int index1 = findCharIndex(square, inchar1);
    int row1 = index1 / 6;
    int col1 = index1 % 6;

    int index2 = findCharIndex(square, inchar2);
    int row2 = index2 / 6;
    int col2 = index2 % 6;

    outchar1 = square[row1][col2];
    outchar2 = square[row2][col1];
}

int findCharIndex(const char square[][6], const char character){
    for (int i = 0; i < 32; i++){
        if (square[i/6][i%6]==character){
            return i;
        }
    }
    return -1;
}

void encode(const char square[][6], const char* prepared, char* encoded){
    bigram(square, *prepared, *(prepared+1), *encoded, *(encoded+1));

    if (!isalnum(*(prepared+2))){
        return;
    }
    else{
        encode(square, prepared+2, encoded+2);
    }
}

void decode(const char square[][6], const char* encoded, char* decoded){
    bigram(square, *encoded, *(encoded+1), *decoded, *(decoded+1));

    if (!isalnum(*(encoded+2))){
        return;
    }
    else{
        encode(square, encoded+2, decoded+2);
    }
}
