# iliyacod@gmail.com
CXX = clang++ 
CXXFLAGS = -std=c++17 -Wall -Wextra

# object files
OBJS = main.o Graph.o Algorithms.o

# object files for tests
TEST_OBJS = tests.o Graph.o Algorithms.o

# the target
all: main test

# main target
main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $(OBJS)

# test target
test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o test $(TEST_OBJS)

# main object file
main.o: main.cpp doctest.h Graph.hpp Algorithms.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# test object file
tests.o: tests.cpp doctest.h Graph.hpp Algorithms.hpp
	$(CXX) $(CXXFLAGS) -c tests.cpp

# graph object file
Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp

# algorithms object file
Algorithms.o: Algorithms.cpp Algorithms.hpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c Algorithms.cpp

# valgrind tests to check for memory leaks
valgrind: main test
	valgrind --leak-check=full ./main
	valgrind --leak-check=full ./test

# clean up 
clean:
	rm -f *.o main test