#ifndef CORRECT_H
#define CORRECT_H

void ascii_to_binary(const char letter, char *output);
char binary_to_ascii(const char *binary);

/* 1
Write a function text to binary(str,binary) which converts a string of characters
(the input parameter str) into a corresponding stream of bits6 (the output string pa-
rameter binary). 
Also write a complementary function binary to text(binary,str)
which converts a stream of bits (the input string parameter binary) into a corresponding
character string (the output parameter str).
*/

void text_to_binary(const char* str, char* binary);
void binary_to_text(const char* binary,char* str);

/* 2
Write a function add error correction(data, corrected) which outputs (in the out-
put string parameter corrected) an error-corrected version of a data bit stream (the
input string parameter data).
*/

void add_error_correction(const char* data, char* correct);

/* 3
Write a function decode(received, decoded) which outputs (in the output string
parameter decoded) an error-corrected version of a received bit stream (the input string
parameter received). 
The function should return the number of errors corrected.
*/

int decode(char* correct, char* decoded, int errors = 0);


#endif
