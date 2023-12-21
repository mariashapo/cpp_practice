substring: main.o substring.o
	g++ -g -Wall main.o substring.o -o substring

main.o: main.cpp substring.h
	g++ -g -c -Wall main.cpp

substring.o: substring.cpp substring.h
	g++ -g -c -Wall substring.cpp

clean:
	rm -f *.o