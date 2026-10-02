# Analysis of Algorithms - Assignment 1: Graph Algorithms

**Author:** Harsha Karimikonda  
**Language:** C++ (C++17, GCC / MinGW-w64 or Clang)  
**Repository:** https://github.com/Harsha-Karimikonda/aoa-assignment-1.git

---

## 1. Project Overview

This project provides an ultra-minimal, efficient implementation of graph algorithms in C++ without any external libraries, adhering strictly to assignment requirements.

### Graph Model Selection
* **Model:** **Undirected Graph**
* **Rationale:**
  1. All 6 simulated graphs (n-cycle, Kn, empty, heap, truncated heap, equivalence mod k) represent symmetric, bidirectional relationships.
  2. In undirected graphs, simple cycle detection ignores trivial parent backtracks (u -> v -> u), returning cycles with >= 3 distinct vertices (length >= 4 with start == end).
  3. Real-world road networks represent undirected physical roadway connections.

### Core Data Structure
* **Adjacency List:** `std::vector<std::vector<int>>` inside `Graph` struct.
* **Complexity:** O(V + 2E) integers memory, O(1) vertex neighbor lookup, optimal CPU cache locality.

---

## 2. Minimal Directory Layout

```
aoa-assignment-1/
├── graph_operations.h       # Graph struct & declarations for the 3 algorithms
├── graph_operations.cpp     # connected_components(), one_cycle(), shortest_paths()
├── graph_simulator.h        # Declarations for all 6 simulated graph generators
├── graph_simulator.cpp      # Implementations of simulated graph generators
├── simulated_test.cpp       # Main driver for simulated tests & scaling benchmarks
├── realgraph_make.h         # Bonus: Real graph ingestion with 3 adjacency criteria
├── realgraph_make.cpp       # Preprocessing & filtering of SNAP road network
├── run_realgraph_make.cpp   # Bonus runner for 1.96M-node real graph
├── benchmark.h              # Minimal cross-platform timer & peak memory profiler
├── results.txt              # Experimental results, metrics, and observations
├── Makefile                 # Minimal build configuration
├── README.md                # Project overview and instructions
├── Karimikonda Harsha assignment1.zip # Submission archive
└── data/
    └── roadNet-CA.txt       # Real dataset (1,965,206 nodes, 2,766,607 edges)
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
# Build both executables:
make

# Run simulated graph tests and benchmarks:
./simulated_test

# Run real graph bonus benchmark (1.96M nodes):
./run_realgraph_make
```

Direct compilation with g++:
```bash
g++ -std=c++17 -O3 graph_operations.cpp graph_simulator.cpp simulated_test.cpp -o simulated_test -lpsapi
g++ -std=c++17 -O3 graph_operations.cpp realgraph_make.cpp run_realgraph_make.cpp -o run_realgraph_make -lpsapi
```
