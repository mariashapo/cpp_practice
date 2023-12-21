maze: maze.o main.o maze.h
	g++ -std=c++11 -g -Wall maze.o main.o -o maze

main.o: main.cpp maze.h
	g++ -std=c++11 -g -c -Wall main.cpp

maze.o: maze.cpp maze.h
	g++ -std=c++11 -g -c -Wall maze.cpp 

clean: 
	rm -f *.o