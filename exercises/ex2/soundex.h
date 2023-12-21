#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <iostream>
#include <cctype>
using namespace std;

void encode(const char* surname, char* soundex);

char* map(const char ch);

bool compare(const char* s1, const char* s2);

int count(const char* surname, const char* sentence);
#endif