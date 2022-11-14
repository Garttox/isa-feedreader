CC = g++
CXXFLAGS = -std=c++11 -Wall -lcrypto -lssl
XMLFLAGS = -I/usr/local/include/libxml2 -I/usr/include/libxml2 -lxml2
EXECUTABLE = feedreader
XLOGIN = xtrlic02

compile:
	@$(CC) -o $(EXECUTABLE) src/*.cpp $(CXXFLAGS) $(XMLFLAGS)

compileFreeBSD:
	@$(CC) -o $(EXECUTABLE) src/*.cpp $(CXXFLAGS) -static-libstdc++ $(XMLFLAGS)

run: compile
	@./$(EXECUTABLE) -f feedfile.txt -a -u -T

valgrind: compile
	@valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE) -f feedfile.txt -aTu

clean:
	@rm -r $(EXECUTABLE) $(XLOGIN).tar

pack:
	tar -cvf $(XLOGIN).tar src/ Makefile README tests/ tests.py

test: compile
	python3 tests.py