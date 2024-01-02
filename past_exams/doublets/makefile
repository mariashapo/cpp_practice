doublets: doublets.o main.o
	g++ -g -Wall doublets.o main.o -o doublets

doublets.o: doublets.cpp doublets.h dictionary.h
	g++ -g -c -Wall doublets.cpp 

main.o: main.cpp doublets.h dictionary.h
	g++ -g -c -Wall main.cpp