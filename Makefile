CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -O0

build: src/main.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/main.cpp src/Node.cpp src/Solution.cpp -o lab_4

test: src/test.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/test.cpp src/Node.cpp src/Solution.cpp -o test