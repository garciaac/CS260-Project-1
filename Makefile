all: main

main: main.o function.o constants.h
	g++ main.o function.o -o main
function.o: function.cpp constants.h
	g++ -c function.cpp

clean: rm -rf *.o 
	rm -rf *.exe
