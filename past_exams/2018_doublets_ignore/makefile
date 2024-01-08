doublets: doublets.o main.o
	g++ -g -Wall -O3 doublets.o main.o -o doublets

doublets.o: doublets.cpp doublets.h dictionary.h
	g++ -g -c -Wall -O3 doublets.cpp 

main.o: main.cpp doublets.h dictionary.h
	g++ -g -c -Wall -O3 main.cpp