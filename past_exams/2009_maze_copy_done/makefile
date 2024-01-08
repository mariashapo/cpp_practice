maze: maze.o main.o maze.h
	g++ -g -Wall maze.o main.o -o maze

main.o: main.cpp maze.h
	g++ -g -c -Wall main.cpp

maze.o: maze.cpp maze.h
	g++ -g -c -Wall maze.cpp 

clean: 
	rm -f *.o