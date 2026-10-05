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
	rm -rf .staging

PACKAGE_NAME = Karimikonda Harsha assignment1.zip

package:
	@rm -rf .staging "$(PACKAGE_NAME)"
	@mkdir -p .staging/include .staging/src
	@cp include/* .staging/include/
	@cp src/* .staging/src/
	@cp Makefile README.md results.txt .staging/
	@cp include/*.h src/*.cpp .staging/
	@cp "include/graph_operations.h" ".staging/graph operations.h"
	@cp "src/graph_operations.cpp" ".staging/graph operations.cpp"
	@cp "include/graph_simulator.h" ".staging/graph simulator.h"
	@cp "src/graph_simulator.cpp" ".staging/graph simulator.cpp"
	@cp "src/simulated_test.cpp" ".staging/simulated test.cpp"
	@cp "include/realgraph_make.h" ".staging/realgraph make.h"
	@cp "src/realgraph_make.cpp" ".staging/realgraph make.cpp"
	@cp "src/run_realgraph_make.cpp" ".staging/run realgraph make.cpp"
	@cd .staging && zip -q -r "../$(PACKAGE_NAME)" .
	@rm -rf .staging
	@echo "Created $(PACKAGE_NAME)"
