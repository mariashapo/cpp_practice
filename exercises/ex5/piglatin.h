#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <fstream>
#include <iostream>

const char VOWELS[] = {'a','e','i','o','u','\0'};

int findFirstVowel(const char* str);

void translateWord(const char* english, char* translated);

void translateStream(std::ifstream& input, std::ostream& cout);

#endif