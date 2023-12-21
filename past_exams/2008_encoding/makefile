playfair: playfair.o main.o
	g++ -g -Wall playfair.o main.o -o playfair
playfair.o: playfair.cpp playfair.h
	g++ -g -c -Wall playfair.cpp
main.o: main.cpp playfair.h
	g++ -g -c -Wall main.cpp
clean:
	rm -f *.o
