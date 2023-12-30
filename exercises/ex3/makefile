words: main.o words.o 
	g++ -g -Wall main.o words.o -o words

main.o: main.cpp words.h
	g++ -g -c -Wall main.cpp

words.o: words.cpp words.h
	g++ -g -c -Wall words.cpp
