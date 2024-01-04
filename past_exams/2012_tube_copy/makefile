tube: tube.o main.o
	g++ -g -Wall tube.o main.o -o tube

tube.o: tube.cpp tube.h
	g++ -g -c -Wall tube.cpp 

main.o: main.cpp tube.h
	g++ -g -c -Wall main.cpp

clean:
	rm *.o