#include <iostream>
#include <cstring>
#include <cctype>
#include "correct.h"

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(const char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(const char *binary);

void ascii_to_binary(const char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(const char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */

// convert text to a binary string
void text_to_binary(const char* str, char* binary){

  // recursive
  if (*str=='\0'){
    *binary = '\0';
    return;
  }
  ascii_to_binary(*str, binary);
  return text_to_binary(++str, binary+8);
}

void binary_to_text(const char* binary, char* str){
  // converts a stream of bits (the input string parameter binary) into a corresponding character string
  if (*binary=='\0'){
    *str = '\0';
    return;
  }
  *str = binary_to_ascii(binary);
  binary_to_text(binary+8, ++str);
}

/*
Write a function add error correction(data, corrected) which outputs (in the out-
put string parameter corrected) an error-corrected version of a data bit stream (the
input string parameter data).
*/

int char_to_num(const char ch){
  return ch - '0';
}

char parity(const char d1, const char d2, const char d3, const char d4 = '0'){
  return ((char_to_num(d1)+char_to_num(d2)+char_to_num(d3)+char_to_num(d4))%2 == 0)?'0':'1';
}

// eg: add_error_correction("0100", correct);
void add_error_correction(const char* data, char* correct){
  if (!isdigit(*data)){
    return;
  }
  // c1 , c2 , d1 , c3 , d2 , d3 , d4
  char d1 = data[0], d2 = data[1], d3 = data[2], d4 = data[3]; 

  char c1 = parity(d1 , d2 , d4 );
  char c2 = parity(d1 , d3 , d4 );
  char c3 = parity(d2 , d3 , d4 );

  correct[0]=c1; correct[1]=c2; correct[2]=d1; 
  correct[3]=c3; correct[4]=d2; correct[5]=d3; 
  correct[6]=d4; correct[7]='\0';
  add_error_correction(data + 4, correct + 7);
}

/*
Write a function decode(received, decoded) which outputs (in the output string
parameter decoded) an error-corrected version of a received bit stream (the input string
parameter received). 
The function should return the number of errors corrected.
*/

int decode(char* received, char* decoded, int errors){
  if (!isdigit(*received)){
    return errors;
  }

  char corrected[8];
  strncpy(corrected, received, 7);
  corrected[7]='\0';
  
  char b1=corrected[0], b2=corrected[1], b3=corrected[2], b4=corrected[3], b5=corrected[4], b6=corrected[5], b7=corrected[6];

  char p1 = parity(b4 , b5 , b6 , b7 );
  char p2 = parity(b2 , b3 , b6 , b7 );
  char p3 = parity(b1 , b3 , b5 , b7 );

  if (!(p1 == '0' && p2 == '0' and p3 == '0')){
    // The position of the bit error is given by the decimal value of the binary number p1p2p3
    char binary_error[9];
    for (int i = 0; i<5; i++){
      binary_error[i] = '0';
    }
    binary_error[5] = p1; binary_error[6] = p2; binary_error[7] = p3; binary_error[8] = '\0';
    // now we have a binary string

    int error_position = binary_to_ascii(binary_error); // use int instead of char to obtain the actual number instead of a character

    // flip the character at the error position
    corrected[error_position - 1] = (corrected[error_position - 1]=='0')?'1':'0';
    errors++;
  }
  // b3 , b5 , b6 , b7
  decoded[0] = corrected[2]; 
  decoded[1] = corrected[4]; 
  decoded[2] = corrected[5]; 
  decoded[3] = corrected[6]; 
  decoded[4] = '\0';

  return decode(received + 7, decoded + 4, errors);
}