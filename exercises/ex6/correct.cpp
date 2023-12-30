#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary);

// this function converts ascii representation of a character to a binary
void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

int binary_to_int(char *binary) {
  int len = strlen(binary), result = 0;
  // starting from the last character
  for (char* i = binary; *i; i++){
    result += (*i=='1')?pow(2, len - 1):0;
    len-=1;
  }
  return result;
}

char binary_to_ascii_numbers(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */

void text_to_binary(const char* str, char* binary){
  if (*str=='\0' || !isalpha(*str)){
    return;
  }
  char binary_temp[9];
  ascii_to_binary(*str, binary_temp);
  strcat(binary, binary_temp);
  text_to_binary(str+1, binary);
}

void binary_to_text(char* binary, char* str){
  if (*binary=='\0'||!isdigit(*binary)){
    str[strlen(str)]='\0';
    return;
  }
  char temp = binary_to_ascii(binary);
  str[strlen(str)]=temp;
  binary_to_text(binary+8, str);
}

char parity(char a, char b, char c){
  return (a + b + c - '0'*3)%2==0?'0':'1';
}

char parity(char a, char b, char c, char d){
  return (a + b + c + d - '0'*4)%2==0?'0':'1';
}

void add_error_correction_char(const char* data, char* corrected){
  char c1,c2,d1,c3,d2,d3,d4;
  d1 = data[0];
  d2 = data[1];
  d3 = data[2];
  d4 = data[3];
  c1 = parity(d1, d2, d4);
  c2 = parity(d1, d3,  d4);
  c3 = parity(d2, d3, d4);
  corrected[0]=c1; corrected[1]=c2;
  corrected[2]=d1; corrected[3]=c3;
  corrected[4]=d2; corrected[5]=d3;
  corrected[6]=d4; corrected[7]='\0';
}

void add_error_correction(const char* data, char* corrected){
  if (*data=='\0' || !isdigit(*data)){
    return;
  }
  add_error_correction_char(data, corrected);
  add_error_correction(data+4, corrected+7);
}

int decode_char(const char* received, char* decoded){
  int result;
  char b[strlen(received)];
  strcpy(b, received);

  char p1 = parity(b[3],b[4],b[5],b[6]); 
  char p2 = parity(b[1],b[2],b[5],b[6]); 
  char p3 = parity(b[0],b[2],b[4],b[6]);

  if(p1 =='0' && p2 =='0' && p3 =='0'){
    result = 0;
  }
  else{
    char error_binary[4];
    error_binary[0]=p1;
    error_binary[1]=p2;
    error_binary[2]=p3;
    error_binary[3]='\0';
    //cout << "<" <<error_binary << ">"<<endl;

    int error_pos = binary_to_int(error_binary);
    //cout << "< error pos: "<< error_pos << ">"<< endl;
    b[error_pos-1]=(b[error_pos-1]=='0')?'1':'0';

    result = 1;
  }

  decoded[0]=b[2];
  decoded[1]=b[4];
  decoded[2]=b[5];
  decoded[3]=b[6];
  decoded[4]='\0';
  return result;
}

int decode(const char* received, char* decoded){
  int count = 0;
  if (!*received){
    return 0;
  }
  count = decode_char(received, decoded);
  count += decode(received + 7, decoded + 4);
  return count;
}