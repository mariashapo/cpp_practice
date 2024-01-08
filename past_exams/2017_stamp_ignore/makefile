stamp: stamp.o main.o
	g++ -g -Wall stamp.o main.o -o stamp -lcrypto 

stamp.o: stamp.cpp stamp.h
	g++ -g -c -Wall stamp.cpp

main.o: main.cpp stamp.h
	g++ -g -c -Wall main.cpp

clean:
	rm *.o