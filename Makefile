CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Iinclude

# On Windows link psapi for GetProcessMemoryInfo
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lpsapi
else
    LDFLAGS =
endif

OBJS_COMMON = src/graph.o src/graph_operations.o src/graph_simulator.o src/realgraph_make.o

all: simulated_test run_realgraph_make

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

simulated_test: $(OBJS_COMMON) src/simulated_test.o
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) src/simulated_test.o -o simulated_test $(LDFLAGS)

run_realgraph_make: $(OBJS_COMMON) src/run_realgraph_make.o
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) src/run_realgraph_make.o -o run_realgraph_make $(LDFLAGS)

clean:
	rm -f src/*.o simulated_test simulated_test.exe run_realgraph_make run_realgraph_make.exe
