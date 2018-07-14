all: main

main: main.o server.o
	g++ -o client_output main.o
	g++ -o serv_output server.o

server.o: server.cpp
	g++ -c server.cpp -o server.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o client_output serv_output


