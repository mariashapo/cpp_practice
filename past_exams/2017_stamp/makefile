stamp: stamp.o main.o
	g++ -g -Wall main.o stamp.o -o stamp -lcrypto

stamp.o: stamp.cpp stamp.h
	g++ -g -c -Wall stamp.cpp -o stamp.o

main.o: main.cpp stamp.h
	g++ -g -c -Wall main.cpp -o main.o
