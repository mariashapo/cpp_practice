#include "braille.h"
#include <cctype>
#include <cstring>
#include <iomanip>

void encoder(const char* sequence, char* braille){
    int offset = 0;
    if (strlen(braille)!=0){
        offset = strlen(braille);
        //braille[offset++]='|';
    }
    for (const char* s = sequence; *s; s++){
        braille[offset]=*s;
        offset++;
    }
    braille[offset]='\0';
}

const char* encoder_switch(const char ch){
    switch (ch){
        case 'a': return "0.....";
        case 'b': return "00....";  
        case 'c': return "0..0..";
        case 'd': return "0..00.";
        case 'e': return "0...0.";
        case 'f': return "00.0..";
        case 'g': return "00.0..";
        case 'h': return "00..0.";
        case 'i': return ".0.0.."; // 
        case 'j': return ".0.00.";
        case 'k': return "0.0...";
        case 'l': return "000...";
        case 'm': return "0.00..";
        case 'n': return "0.000."; //
        case 'o': return "0.0.0.";
        case 'p': return "0000..";
        case 'q': return "00000."; //
        case 'r': return "000.0.";
        case 's': return ".000..";
        case 't': return ".0000.";
        case 'u': return "0.0..0";
        case 'v': return "000..0";
        case 'w': return ".0.000";
        case 'x': return "0.00.0";
        case 'y': return "0.0000";
        case 'z': return "0.0.00";
        case '.': return ".0..00";
        case ',': return ".0....";
        case ';': return ".00..";
        case '-': return "..0..0";
        case '!': return ".00.0.";
        case '?': return ".00..0";
        case '(': 
        case ')': return ".00.00";
        default: return "empty";
    }
}

int encode_character(const char ch, char* braille, bool reset){
    if (reset){
        braille[0]='\0';
    }

    char my_char = ch;
    int size = 0;
    const char* encoded_char;

    if (isupper(my_char)){
        encoder( ".....0", braille);
        size++;
    }

    else if (isdigit(my_char)){
        if (my_char=='0'){
            my_char = 'j';
        }
        else {  
            my_char = my_char -('1'-'a');
        }
        encoder("..0000", braille);
        size++;
    }
    encoded_char = encoder_switch(tolower(my_char));

    encoder(encoded_char, braille);
    size++;

    return size;
}

void encode(const char* phrase, char* braille, bool reset){
    if (*phrase){
        encode_character(*phrase, braille,reset);
        reset = false;
        encode(phrase + 1, braille, reset);
    }
    else{
        return;
    }
}

void print_braille(const char* phrase, ostream& cout){
    char braille[512];

    encode(phrase, braille);

    // remove the upper case 
    for (int offset = 0; offset <3; offset++){ // row iter
        for (const char* b = braille; *b; b+=6){  // col iter
            cout << *(b + offset) << *(b + offset + 3);
            if (*(b+6)){ // check if we are on the last col, to skip to the next line
                cout << " ";
            }
            else{
                cout << endl;
            }
        }    
    }
    cout << left;
    // finished printing the braille rows here:
    for (const char* p = phrase; *p; p++){
        if (isupper(*p)||isdigit(*p)){
            cout << setw(3) << ' ';
        }
        cout << setw(3) << *p;
    }
    cout << endl;
}