braille: braille.o main.o
	g++ -g -Wall braille.o main.o -o braille

main.o: main.cpp braille.h
	g++ -g -c -Wall main.cpp

braille.o: braille.cpp braille.h
	g++ -g -c -Wall braille.cpp