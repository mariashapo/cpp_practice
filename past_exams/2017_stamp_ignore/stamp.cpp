#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
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

//q1
/*
Write an integer-valued function leading zeros(digest) which takes as its parameter a
hexadecimal string representing a hash digest and returns the number of leading zeros it
beings with. If the input string contains characters that are not valid hexadecimal digits
(i.e. ’0’ to ’9’ and ’a’ to ’f’) then the function should return -1.
*/

//eg: leading_zeros("60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5")
int leading_zeros(const char* digest){
  for (const char* c = digest; *c; c++){
    if (!isxdigit(*c)){
      return -1;
    }
  }
  return strspn(digest, "0");
}

bool open_file(ifstream& in, const char* filename){
  in.open(filename);
  if (!in.is_open()){
    return false;
  }
  else{
    return true;
  }
}


//q2
/*
Write a boolean function file to SHA1 digest(filename, digest)
* returns in digest the SHA1 digest of the contents of the file with name filename. 
* if the file cannot be opened
then the function should return false with digest set to “error”; otherwise return true.
*/
//eg: file_to_SHA1_digest("message1.txt", digest);

bool file_to_SHA1_digest(const char* filename, char* digest){
  ifstream in;
  if(!open_file(in, filename)){
    strcpy(digest, "error");
    return false;
  }
  
  in.seekg(0, ios::end);
  int length = in.tellg();
  in.seekg(0, ios::beg);

  char buffer[length + 1];
  /*
  char line[512];
  buffer[0] = '\0';

  in.getline(line, 511);
  strcat(buffer, line);

  while (!in.eof()) {
      strcat(buffer, "\n");
      in.getline(line, 511);
      strcat(buffer, line);
  }
  */
  char my_char;
  int position = 0;

  in.get(my_char);
  while (!in.eof()) {
    buffer[position++]=my_char;
    in.get(my_char);
  }
  buffer[position] = '\0';
  in.close();
  
  text_to_SHA1_digest(buffer, digest); 
  return true;
}

//q3
/*
Write a Boolean function make header(recipient, filename, header) which prepares a
header for a given email message. Here recipient is a read-only input parameter specifying
the email address of the message recipient, filename is a read-only input parameter spec-
ifying the file containing the message body, and header is an output parameter containing
the header in form:
<recipient email address>:<SHA1 digest of message body>:<counter>
The return value of the function should be false if the file containing the message body,
cannot be read, or if a valid counter is not found after 10 million attempts; otherwise the
function should return true.
*/


void increment_counter(char* digest, int attempts){
  char int_str[100];
  char* last_position = strrchr(digest, ':');
  *(last_position+1)='\0';
  sprintf(int_str, "%d", attempts);
  strcat(digest, int_str);
}

bool make_header(const char* email, const char* file, char* header){
  char digest[41];
  int attempts = 1;

  // <recipient email address>:<SHA1 digest of message body>:<counter>
  header[0]='\0';
  strcat(header, email);
  strcat(header, ":");
  file_to_SHA1_digest(file, digest);
  strcat(header, digest);
  strcat(header, ":0");
  // base case header formed
  cout << header << endl;
  // generate the first digest of the header
  // we can reuse the same digest
  text_to_SHA1_digest(header, digest);

  while (leading_zeros(digest) != 5){
    if (attempts >= 10000000){
      return false;
    }
    increment_counter(header, attempts);
    text_to_SHA1_digest(header, digest);
    attempts++;
  }
  return true;
} 

//q4
/*
Write a function check header(email address,header,filename) which features three
input parameters: the user’s email address, a received message header and the name of a file
containing the text of a received email body. The return value should of enumerated type
MessageStatus (see stamp.h), which can be printed in a human-readable manner using the
message status(...) internal helper function in main.cpp.
*/

/*
check_header("wjk@imperial.ac.uk",
    "wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889",
    "message1.txt");
    */

/*
 Confirm that the header is in the correct format (i.e. three fields separated by ’:’).
 Check the recipient mentioned in the first field of the header matches the expected recipient.
 Check that the SHA1 digest of the message body given in the header matches the actual
SHA1 digest of the received message.
 Check that the SHA1 digest of the header is an acceptable header.
*/
/*
bool correct_header(const char* header){
  char* first_break = strchr(header, ':');
  char* second_break = strrchr(header, ':');

  for (const char* c; ; c++){

  }
}

MessageStatus check_header(const char* email, const char* header, const char* filename){
  char digest[41];
  int email_len = strlen(email);
  if (strncmp(header, email, email_len)!=0){
    return WRONG_RECIPIENT;
  }

  text_to_SHA1_digest(header, digest);
  if (leading_zeros(digest) != 5){
    return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}
*/