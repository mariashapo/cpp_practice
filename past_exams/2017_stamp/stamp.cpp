#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

bool valid_hex(const char c){
  if (('0'<=c && c<='9') || ('a'<=c&&c<='f')){
    return true;
  }
  else{
    return false;
  }
}

int leading_zeros(const char* digest){
  int count = 0;
  // check if all digits are valid hex digits
  for (const char* d = digest; *d; d++){
    if (!valid_hex(*d)){
      return -1;
    }
  }
  for (const char* d = digest; *d; d++){
    if (*d == '0'){
      count++;
    }
    else {
      break;
    }
  }
  return count;
}

bool file_to_SHA1_digest(const char* file, char* digest){
  ifstream in(file);
  char my_char;
  string text;

  if (!in.is_open()){
    strcpy(digest, "error");
    return false;
  }

  in.get(my_char);
  while (!in.eof()){
    text+=my_char;
    in.get(my_char);
  }
  in.close();

  text_to_SHA1_digest(text.c_str(), digest);

  return true;
}

bool file_to_SHA1_digest_c(const char* file, char* digest){
  ifstream in (file);
  if (!in.is_open()){
    strcpy(digest, "error");
    return false;
  }
  in.seekg(0, ios::end);
  int len = in.tellg();
  in.seekg(0, ios::beg);
  char buffer [len+1];
  in.read(buffer, len);
  buffer[len]='\0';
  in.close();
  text_to_SHA1_digest(buffer, digest);
  return true;
}

bool find_semicol(const char* header, int& first, int& second){
  second = first = -1;
  int count = 0;
  for (int i = 0; *(header + i); i++){
    if (*(header+i)==':'){
      switch(count){
        case 0: first = i; break;
        case 1: second = i; break;
        case 2: return false;
      }
      count ++;
    }
  }
  if (first == -1 || second == -1){
    return false;
  }
  else{
    return true;
  }
}

void append_counter(char* header, int counter){
  int first, second;
  char counter_str [10];
  find_semicol(header, first, second);
  sprintf(counter_str, "%d", counter);
  strcpy(header+second+1, counter_str);
  return;
}

bool make_header(const char* email, const char* file, char* header){
  header[0]='\0';
  char digest[41];
  int counter = 0 ;

  // read in from the file and convert it into digest
  if (!file_to_SHA1_digest(file, digest)){
    strcpy(digest, "error");
    digest[strlen("error")]='\0';
    return false;
  }

  strcat(header, email);
  strcat(header, ":");
  strcat(header, digest);
  strcat(header, ":");
  cout << header << endl;

  while (counter <10000000){
    append_counter(header, counter);
    text_to_SHA1_digest(header, digest);

    if (leading_zeros(digest)==5){
      return true;
    }
    //cout << header << endl;
    counter ++;
  }
  return false;
}

MessageStatus check_header(const char* email, const char* header, const char* filename){
  int first, second;
  char digest[41], digest_check[41], digest_header[41];
  if (!find_semicol(header, first, second)){
    return INVALID_HEADER;
  }

  if (strncmp(header,email,first)!=0){
    return WRONG_RECIPIENT;
  }

  // extract digest
  const char* digest_start = header + first + 1;
  int len_of_digest = second - first - 1;
  if (len_of_digest!=40){
    return INVALID_MESSAGE_DIGEST;
  }
  strncpy(digest, digest_start, len_of_digest);
  file_to_SHA1_digest(filename, digest_check);
  text_to_SHA1_digest(header, digest_header);
  

  if (strcmp(digest, digest_check)!=0){
    return INVALID_MESSAGE_DIGEST;
  }

  if (leading_zeros(digest_header)!=5){
    return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}