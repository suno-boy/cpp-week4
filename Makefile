main.exe: main.o
	g++ main.o -o main.exe 
main.o: main.cpp
	g++ -Werror -c main.cpp
