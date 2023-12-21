#include <cstring>
#include "substring.h"

int substring_position(const char* substr, const char* str, int offset){
    if (is_prefix(substr,str)){
        return (offset);
    }
    else if (*str=='\0'){
        return -1;
    }
    else {
        return substring_position(substr, str+1, offset+1);
    }
}

bool is_prefix(const char* substr, const char* str){
    if (*substr == '\0'){
        return true;
    }
    else if(*substr == *str){
        return is_prefix(substr+1,str+1);
    }
        return false;
}

int substring_position2(const char* substr, const char* str){
    return (strstr(str, substr) >= str )? strstr(str, substr) - str: -1;
}