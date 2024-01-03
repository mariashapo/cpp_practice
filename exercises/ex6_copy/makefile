correct: correct.o main.o
	g++ -g correct.o main.o -o correct

correct.o: correct.cpp correct.h
	g++ -g -c correct.cpp 

main.o: main.cpp correct.h
	g++ -g -c main.cpp