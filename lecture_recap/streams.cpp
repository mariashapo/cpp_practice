/* Program 5.7.1 from C++ Programming Lecture notes  */

/* Author: Rob Miller and William Knottenbelt
   Program last changed: 30th September 2023 */

/* This program creates a file with five integers, 
then counts the number of integers and the number of 
characters in the file*/ 

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	char character;
	int number = 51;
	int	count = 0;
	ofstream out_stream;  
	ifstream in_stream1;   /* Stream for counting integers. */
	ifstream in_stream2;   /* Stream for counting characters. */

	/* Create the file */
	out_stream.open("Integers");
	for (count = 1 ; count <= 5 ; count++) 
		out_stream << number++ << ' ';
	out_stream.close();	
	
	/* Count the integers */
	in_stream1.open("Integers");
	count = 0;
	in_stream1 >> number;
	while (!in_stream1.fail())
	{
		count++;
		in_stream1 >> number;
	}
	cout << "There are " << count << " integers in the file,\n";
	
	/* Count the characters */
	in_stream1.clear();
    in_stream1.seekg(0);
	count = 0;
	in_stream1 >> character;
	while (!in_stream1.fail())
	{
		count++;
		in_stream1 >> character;
	}
	cout << "represented using " << count << " characters.\n";

    count = 0;
    in_stream1.clear();
    in_stream1.seekg(0);
	count = 0;
    in_stream1.get(character);
    while (!in_stream1.fail()){
        count++;
        in_stream1.get(character);
    }

    cout << "represented using " << count << " characters (get).\n";

	return 0;
}
