laser: laser.o main.o
	g++ -g -Wall laser.o main.o -o laser

laser.o: laser.cpp laser.h
	g++ -g -c -Wall laser.cpp 

main.o: main.cpp laser.h
	g++ -g -c -Wall main.cpp 
