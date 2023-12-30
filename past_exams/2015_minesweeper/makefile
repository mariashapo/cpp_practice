mines: minesweeper.cpp main.cpp
	g++ -g -Wall minesweeper.cpp main.cpp -o mines

minesweeper.o: minesweeper.cpp, minesweeper.h
	g++ -g -c -Wall minesweeper.cpp 

main.o: main.cpp, minesweeper.h
	g++ -g -c -Wall main.cpp 