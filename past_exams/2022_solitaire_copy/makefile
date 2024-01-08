solitaire: solitaire.o main.o
	g++ -g -Wall solitaire.o main.o -o solitaire

solitaire.o: solitaire.cpp solitaire.h
	g++ -g -c -Wall solitaire.cpp

main.o: main.cpp solitaire.h
	g++ -g -c -Wall main.cpp