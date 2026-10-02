# Analysis of Algorithms - Assignment 1: Graph Algorithms

**Author:** Harsha Karimikonda  
**Language:** C++ (C++17, GCC / MinGW-w64 or Apple Clang)  
**Repository:** https://github.com/Harsha-Karimikonda/aoa-assignment-1.git

---

## 1. Project Overview

This project provides an efficient, modular implementation of fundamental graph algorithms in C++ without external graph libraries, strictly adhering to assignment specifications.

### Graph Model Selection
* **Model:** **Undirected Graph**
* **Rationale:**
  1. All 6 simulated graphs (n-cycle, Kn, empty, heap, truncated heap, equivalence mod k) represent symmetric, bidirectional relationships.
  2. In undirected graphs, simple cycle detection ignores trivial parent backtracks (u -> v -> u), returning cycles with >= 3 distinct vertices (length >= 4 with start == end).
  3. Real-world road networks represent undirected physical roadway connections.

### Core Data Structure
* **Adjacency List:** `std::vector<std::vector<int>>` inside `Graph` struct (`include/graph_operations.h`).
* **Complexity:** O(V + 2E) integers memory, O(1) vertex neighbor lookup, optimal CPU cache locality.

---

## 2. Organized Project Structure

```
aoa-assignment-1/
├── include/
│   ├── graph_operations.h       # Graph struct & declarations for the 3 algorithms
│   ├── graph_simulator.h        # Declarations for all 6 simulated graph generators
│   ├── realgraph_make.h         # Bonus: Real graph ingestion with 3 adjacency criteria
│   └── benchmark.h              # Cross-platform timer & peak memory profiler (Windows/macOS/Linux)
├── src/
│   ├── graph_operations.cpp     # connected_components(), one_cycle(), shortest_paths()
│   ├── graph_simulator.cpp      # Implementations of simulated graph generators
│   ├── simulated_test.cpp       # Main driver for simulated tests & scaling benchmarks
│   ├── realgraph_make.cpp       # Preprocessing & filtering of SNAP road network
│   └── run_realgraph_make.cpp   # Bonus runner for 1.96M-node real graph
├── docs/
│   └── results.txt              # Experimental results, metrics, and observations
├── data/
│   └── roadNet-CA.txt           # Real dataset (1,965,206 nodes, 2,766,607 edges)
├── Makefile                     # Cross-platform build configuration (Windows & macOS)
├── README.md                    # Project overview and instructions
├── results.txt                  # Root copy of experimental report
└── Karimikonda Harsha assignment1.zip # Submission archive
```

---

## 3. Implemented Functions

1. **`connected_components(const Graph& g)`**:
   - Iterative stack-safe DFS using `std::stack<int>`. Prevents stack overflow on 1M+ nodes.
   - Time Complexity: O(V + E) | Space: O(V).

2. **`one_cycle(const Graph& g)`**:
   - DFS with tri-state coloring and predecessor tracking.
   - Reconstructs cycle of >= 3 distinct vertices (`[v, ..., u, v]`). Returns `{}` if acyclic.
   - Time Complexity: O(V + E) | Space: O(V).

3. **`shortest_paths(const Graph& g, int source)`**:
   - Dijkstra's algorithm with min-heap (`std::priority_queue`).
   - Unweighted edges (weight = 1).
   - Reconstructs path list `sp[v]` from $v$ back to $source$ for each reachable vertex.
   - Time Complexity: O((V + E) log V) | Space: O(V + sum of path lengths).

---

## 4. How to Compile and Run

```bash
# Build both executables with Make:
make

# Run simulated graph tests and benchmarks:
./simulated_test

# Run real graph bonus benchmark (1.96M nodes):
./run_realgraph_make
```

Direct compilation with g++ / clang++:
```bash
# Windows:
g++ -std=c++17 -O3 -Iinclude src/graph_operations.cpp src/graph_simulator.cpp src/simulated_test.cpp -o simulated_test -lpsapi
g++ -std=c++17 -O3 -Iinclude src/graph_operations.cpp src/realgraph_make.cpp src/run_realgraph_make.cpp -o run_realgraph_make -lpsapi

# macOS / Linux:
clang++ -std=c++17 -O3 -Iinclude src/graph_operations.cpp src/graph_simulator.cpp src/simulated_test.cpp -o simulated_test
clang++ -std=c++17 -O3 -Iinclude src/graph_operations.cpp src/realgraph_make.cpp src/run_realgraph_make.cpp -o run_realgraph_make
```
