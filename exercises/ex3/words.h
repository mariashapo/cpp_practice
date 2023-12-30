#include <cstring>

#ifndef WORDS_H
#define WORDS_H

//copies str1 into str2 backwards.
void reverse(const char* str1, char* str2);

bool compare(const char* str1, const char* str2);

bool palindrome(const char* str);

bool anagram(const char* str1, const char* str2);

#endif