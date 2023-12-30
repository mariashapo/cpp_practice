#include <iostream>
#include <cctype>
#include "words.h"
using namespace std;

//copies str1 into str2 backwards.
void reverse(const char* str1, char* str2){
    int len = strlen(str1);
    /*
    if (!str2[len]){
        cerr << "too long" << endl;
    }*/
    for (int i = 0; i<len; i++){
        str2[i]=str1[len-1-i];
    }
    str2[len]='\0';
}

bool compare(const char* str1, const char* str2){
    while (!isalpha(*str1) && *str1!='\0'){
        str1++;
    }
    while (!isalpha(*str2) && *str2!='\0'){
        str2++;
    }
    //cout << "<" << *str1 << " " << *str2 << ">" << endl;
    if (*str1=='\0' || *str2=='\0'){
        if (*str1==*str2){
            return true;
        }
        else{
            return false;
        }
    }
    if (tolower(*str1)!=tolower(*str2)){
        return false;
    }
    else{
        return compare(++str1, ++str2);
    }
}

bool palindrome(const char* str){
    int len = strlen(str);
    char str_reversed[len+1];
    reverse(str, str_reversed);
    return compare(str, str_reversed);
}

bool erase(const char c, char* str){
    for (char* c2 = str; *c2 ;c2++){
        if (c == tolower(*c2)){
            *c2 = ' ';
            return true;
        }
    }
    return false;
}

bool anagram(const char* str1, const char* str2){
    // we need to track which letters we have used already
    char str2_copy[strlen(str2)+1];
    strcpy(str2_copy,str2);
    for (const char* c = str1; *c; c++){
        if (!isalpha(*c)){
            continue;
        }
        if (!erase(tolower(*c), str2_copy)){
            return false;
        }
    }

    for (char* c = str2_copy; *c; c++){
        if (isalpha(*c)){
            return false;
        }
    }

    return true;
}