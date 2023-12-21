river: river.o main.o
	g++ -g -Wall main.o river.o -o river
river.o: river.cpp river.h
	g++ -g -c -Wall river.cpp
main.o: main.cpp river.h
	g++ -g -c -Wall main.cpp