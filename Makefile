CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -O0

.PHONY: all build debug automate test

all: build debug automate test

build: src/main.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/main.cpp src/Node.cpp src/Solution.cpp -o lab_4

debug: src/main.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/main.cpp src/Node.cpp src/Solution.cpp -DDEBUG -o debug

automate: src/automate.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/automate.cpp src/Node.cpp src/Solution.cpp -DDEBUG -o automate

test: src/test.cpp src/Node.cpp include/Node.h src/Solution.cpp include/Solution.h
	$(CXX) $(CXXFLAGS) src/test.cpp src/Node.cpp src/Solution.cpp -o test