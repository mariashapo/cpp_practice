#ifndef BRAILLE_H
#define BRAILLE_H

#include <iostream>
using namespace std;

int encode_character(const char ch, char* braille, bool reset = true);

void encode(const char* phrase, char* braille, bool reset = true);

void print_braille(const char* phrase, ostream& cout);

#endif