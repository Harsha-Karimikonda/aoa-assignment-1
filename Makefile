CXX ?= g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Iinclude

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lpsapi
else
    LDFLAGS =
endif

all: simulated_test run_realgraph_make

simulated_test: src/graph_operations.cpp src/graph_simulator.cpp src/simulated_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run_realgraph_make: src/graph_operations.cpp src/realgraph_make.cpp src/run_realgraph_make.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f simulated_test simulated_test.exe run_realgraph_make run_realgraph_make.exe

