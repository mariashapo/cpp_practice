gogen: gogen.o main.o
	g++ -g -Wall gogen.cpp main.cpp -o gogen

gogen.o: gogen.cpp gogen.h mask.h common.h
	g++ -g -c -Wall gogen.cpp

main.o: main.cpp gogen.h mask.h common.h
	g++ -g -c -Wall main.cpp

clean:
	rm *.o