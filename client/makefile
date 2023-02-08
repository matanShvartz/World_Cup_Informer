CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

EchoClient: bin/ConnectionHandler.o bin/echoClient.o
	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/StompProtocol.o bin/Game.o bin/Exit.o bin/Frame.o bin/Join.o bin/Login.o bin/Logout.o bin/Report.o bin/Summary.o bin/KeyboardThread.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/StompProtocol.o bin/Game.o bin/Exit.o bin/Frame.o bin/Join.o bin/Login.o bin/Logout.o bin/Report.o bin/Summary.o bin/KeyboardThread.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/KeyboardThread.o: src/KeyboardThread.cpp
	g++ $(CFLAGS) -o bin/KeyboardThread.o src/KeyboardThread.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/Game.o: src/Game.cpp
	g++ $(CFLAGS) -o bin/Game.o src/Game.cpp

bin/Exit.o: src/Actions/Exit.cpp
	g++ $(CFLAGS) -o bin/Exit.o src/Actions/Exit.cpp

bin/Frame.o: src/Actions/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Actions/Frame.cpp

bin/Join.o: src/Actions/Join.cpp
	g++ $(CFLAGS) -o bin/Join.o src/Actions/Join.cpp

bin/Login.o: src/Actions/Login.cpp
	g++ $(CFLAGS) -o bin/Login.o src/Actions/Login.cpp

bin/Logout.o: src/Actions/Logout.cpp
	g++ $(CFLAGS) -o bin/Logout.o src/Actions/Logout.cpp

bin/Report.o: src/Actions/Report.cpp
	g++ $(CFLAGS) -o bin/Report.o src/Actions/Report.cpp

bin/Summary.o: src/Actions/Summary.cpp
	g++ $(CFLAGS) -o bin/Summary.o src/Actions/Summary.cpp



.PHONY: clean
clean:
	rm -f bin/*
	