CXX ?= g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lpsapi
else
    LDFLAGS =
endif

all: simulated_test run_realgraph_make

simulated_test: graph_operations.cpp graph_simulator.cpp simulated_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run_realgraph_make: graph_operations.cpp realgraph_make.cpp run_realgraph_make.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f simulated_test simulated_test.exe run_realgraph_make run_realgraph_make.exe
