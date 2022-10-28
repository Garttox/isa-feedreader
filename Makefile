CC = g++
CXXFLAGS = -std=c++11 -Wall -I/usr/include/libxml2 -lxml2 -lcrypto -lssl
EXECUTABLE = feedreader

compile:
	@$(CC) -o $(EXECUTABLE) src/*.cpp $(CXXFLAGS)

run: compile
	@./$(EXECUTABLE)