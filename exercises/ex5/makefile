piglatin: piglation.o main.o
	g++ -g -Wall piglatin.o main.o -o piglatin

piglation.o: piglatin.cpp piglatin.h
	g++ -g -c -Wall piglatin.cpp

main.o: main.cpp piglatin.h
	g++ -g -c -Wall main.cpp