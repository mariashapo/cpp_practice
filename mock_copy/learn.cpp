#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void read(ifstream& in){
    char ch;
    string line;
    for (int i =0 ; i<10 ; i++){
        getline(in, line);
        cout << line << endl;
    }
    in.get(ch);
    for (int i =0 ; i<10 ; i++){
        cout<<ch;
        in.get(ch);
    }
    cout << endl;
}

int main(){
    //const char* a = "hello";
    //const char* b = "hello";
    ifstream in("words.dat");
    read(in);
    in.close();
    return 0;
}