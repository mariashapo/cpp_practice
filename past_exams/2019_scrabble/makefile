scrabble: scrabble.o main.o
	g++ -g -Wall scrabble.o main.o -o scrabble

scrabble.o: scrabble.cpp scrabble.h
	g++ -g -c -Wall scrabble.cpp 

main.o: main.cpp scrabble.h
	g++ -g -c -Wall main.cpp 