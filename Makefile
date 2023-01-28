networkmap:main
	g++ main.o operation.o command.o -o networkmap -std=c++11
main:command
	g++ -c main.cpp -o main.o -std=c++11
command:operation
	g++ -c command.cpp -o command.o -std=c++11
operation:
	g++ -c operation.cpp -o operation.o -std=c++11