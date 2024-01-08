#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <climits>
#include "spell.h"

using namespace std;



int frequency(const char* word){
  ifstream in("words.dat");
  int count;
  char buffer[MAX_LENGTH];
  in >> count;
  in >> buffer;
  while (!in.eof()){
    if (strcmp(word, buffer)==0){
      in.close();
      return count;
    }
    in >> count;
    in >> buffer;
  }
  in.close();
  return 0;
}

int dist(const char* a, const char* b, int i, int j, int total = 0, int limit = INT_MAX);

int edit_distance(const char* a, const char* b){
  int i = strlen(a);
  int j = strlen(b);
  return (dist(a, b, i, j));
}

int edit_distance_bonus(const char* a, const char* b){
  int i = strlen(a);
  int j = strlen(b);
  return (dist(a, b, i, j));
}




int min(int a1, int a2, int a3, int a4 = INT_MAX){
   int array[4]={a1, a2, a3, a4};
   int min_val = a1;
   for (int i=1;i<4;i++){
    if (array[i]<min_val){
      min_val=array[i];
    }
   }
   return min_val;
}

int indicator(const char* a, const char* b, int i, int j){
   return (a[i-1]==b[j-1])? 0 : 1;
}

int dist(const char* a, const char* b, int i, int j, int total, int limit){

  if (total>=limit){
    return limit;
  }

  if (i==0){
    return j;
  }
  else if (j==0){
    return i;
  }
  else{
    int a1,a2,a3,a4;
    a1 = dist(a, b, i-1,j, total+1, limit)+1;
    // total + 1 because this branch indicates there is a difference
    // the current thing we are considering involves an edit
    a2 = dist(a, b, i,j-1, total+1, limit)+1;
    a3 = dist(a, b, i-1,j-1, total+indicator(a,b,i,j), limit)+indicator(a,b,i,j);
    // total only increseases if indicator(a,b,i,j) is in fact not 0
    if(i>1 && j>1 && a[i-1]==b[j-2] && a[i-2]==b[j-1]){
      a4 = dist(a, b, i-2,j-2, total+1, limit)+1;

      int dist = min(a1, a2, a3, a4);
      
      return min(a1, a2, a3, a4);
    }
    else{
      return min(a1, a2, a3);
    }
  }
}

bool edit_away(const char* word, char* fixed, int d){
  ifstream in("words.dat");
  bool found = false;
  int count, max_freq=0;
  char fixed_temp[MAX_LENGTH];
  in >> count;
  in >> fixed_temp;
  while (!in.eof()){
    if (dist(word, fixed_temp, strlen(word), strlen(fixed_temp), 0, 3)==d){
      if (count>max_freq){
        max_freq = count;
        strcpy(fixed, fixed_temp);
        found = true;
      }
    }
    in >> count;
    in >> fixed_temp;
  }
  in.close();
  return found;
}

bool spell_correct(const char* word, char* fixed){
  if (frequency(word)>0){
    strcpy(fixed,word);
    return false;
  }
  else if (edit_away(word, fixed, 1)){
    return true;
  }
  else if (edit_away(word, fixed, 2)){
    return true;
  }
  else{
    strcpy(fixed,word);
    return false;
  }
}



/*=========================ARCHVIE=========================*/
int char_to_int(char* occurence){
  //measure the length of the string
  int answer = 0;
  int len = strlen(occurence);
  for (int k = 0; k<len; k++){
    answer+=(pow(10,k)*(occurence[len-1-k]-'0'));
  }
  return answer;
}

int frequency_archive(const char* word){
  // will store the number of times a word occurs
  char occurence[100];
  char ch;
  ifstream in("words.dat");
  in.get(ch);
  while (!in.eof()){
    int i1 = 0;
    // get the next character until it is zero
    while (!isdigit(ch)){
      in.get(ch);
      if (in.eof()){
        return 0;
      }
    }
    // get the next character until it is a white space
    while (ch!=' '){
      occurence[i1]=ch;
      in.get(ch);
      i1 ++;
    }
    // end the character array 
    occurence[i1]='\0';
    
    in.get(ch);

    // while the first letter does match our word
    int i2 = 0;
    while (isalpha(ch)){
      if (word[i2]!=ch){
	      break;
      }
      i2++;
      in.get(ch);
      if (word[i2]=='\0' && !isalpha(ch)){
	      return char_to_int(occurence);
      }
    }
  }
  return 0;
}

