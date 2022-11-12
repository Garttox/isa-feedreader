CC = g++
CXXFLAGS = -std=c++11 -Wall -I/usr/include/libxml2 -lxml2 -lcrypto -lssl
EXECUTABLE = feedreader

compile:
	@$(CC) -o $(EXECUTABLE) src/*.cpp $(CXXFLAGS)

run: compile
	@./$(EXECUTABLE) -f feedfile.txt -a -u -T

valgrind: compile
	@valgrind --leak-check=summary ./$(EXECUTABLE) -f feedfile.txt