stamp: stamp.o main.o
	g++ -g -Wall main.o stamp.o -o stamp -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

stamp.o: stamp.cpp stamp.h
	g++ -g -c -Wall stamp.cpp -I/opt/homebrew/opt/openssl@3/include -o stamp.o

main.o: main.cpp stamp.h
	g++ -g -c -Wall main.cpp -o main.o
