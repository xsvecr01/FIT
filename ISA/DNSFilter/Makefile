default: main.o FileParser.o SocketClient.o SocketServer.o
	g++ -Wall main.o FileParser.o SocketClient.o SocketServer.o -o dns -lpthread

main.o: main.cpp
	g++ -c main.cpp

FileParser.o: FileParser.cpp FileParser.hpp
	g++ -c FileParser.cpp

SocketClient.o: SocketClient.cpp SocketClient.hpp
	g++ -c SocketClient.cpp

SocketServer.o: SocketServer.cpp SocketServer.hpp
	g++ -c SocketServer.cpp

clean:
	rm *.o