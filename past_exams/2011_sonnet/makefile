sonnet: sonnet.o main.o
	g++ -g -Wall sonnet.o main.o -o sonnet

sonnet.o: sonnet.cpp sonnet.h
	g++ -g -c -Wall sonnet.cpp

main.o: main.cpp sonnet.h
	g++ -g -c -Wall main.cpp