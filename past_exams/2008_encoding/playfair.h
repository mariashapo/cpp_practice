#ifndef PLAYFAIR_H
#define PLAYFAIR_H

void prepare(const char* input, char* output);

void grid(const char* codeword, char square[][6]);

void bigram(const char square[][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2);

void encode(const char square[][6], const char* prepared, char* encoded);

void decode(const char square[][6], const char* encoded, char* decoded);

// Helper functions:
//check if a character already occured on the board
bool isNew(const char character, const char square[][6], int index);

// insert a character into the board at a given index
void insert(const char character, char square[][6], int& index);

// gind the index of character
int findCharIndex(const char square[][6], const char character);

#endif