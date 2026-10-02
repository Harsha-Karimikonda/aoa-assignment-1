# Analysis of Algorithms - Assignment 1: Graph Algorithms

**Author:** Harsha Karimikonda  
**Language:** C++ (C++17, GCC / MinGW-w64 or Clang)  
**Repository:** https://github.com/Harsha-Karimikonda/aoa-assignment-1.git

---

## 1. Project Overview

This project implements fundamental graph algorithms and simulated data generators from scratch using C++ STL basic data structures (`std::vector`, `std::map`, `std::stack`, `std::queue`, `std::pair`), strictly adhering to the prohibition against external graph libraries (such as Boost Graph Library).

### Graph Model Selection
* **Model:** **Undirected Graph**
* **Rationale:**
  1. All 6 simulated graphs ($n$-cycle, complete graph $K_n$, empty graph, binary heap, truncated heap, and equivalence mod $k$) represent bidirectional, symmetric connectivity.
  2. Cycle detection in an undirected graph rigorously excludes trivial backtracks to immediate parents ($u \to v \to u$), returning simple cycles with $\ge 3$ distinct vertices ($\ge 4$ vertices starting and ending with the same vertex).
  3. Real-world road networks represent undirected physical road segments.

### Core Data Structure
* **Adjacency List:** Encapsulated in `Graph` class via `std::vector<std::vector<int>>`.
* **Space Complexity:** $O(V + 2E)$ integers with optimal CPU cache locality and zero pointer overhead.
* **Traversal Complexity:** $O(1)$ vertex lookup, $O(\text{deg}(u))$ neighbor traversal.

---

## 2. Directory Structure

```
aoa-assignment-1/
├── include/
│   ├── graph.h                 # Graph data structure declaration
│   ├── graph_operations.h      # Declarations of the 3 required algorithms
│   ├── graph_simulator.h       # Simulated graph generators (all 6 implemented)
│   ├── realgraph_make.h        # 1M+ real graph ingestion with 3 criteria (Bonus)
│   └── benchmark_utils.h       # High-res timer & OS peak memory profiler (Windows/POSIX)
├── src/
│   ├── graph.cpp               # Graph class implementation
│   ├── graph_operations.cpp    # connected_components(), one_cycle(), shortest_paths()
│   ├── graph_simulator.cpp     # Simulated graph generators implementation
│   ├── realgraph_make.cpp      # Real-world loaders (Criterion 1, 2, and 3)
│   ├── simulated_test.cpp      # Main benchmark driver for simulated graphs
│   └── run_realgraph_make.cpp  # Main benchmark driver for 1M+ real graph
├── data/
│   └── roadNet-CA.txt          # Stanford SNAP California Road Network (1.96M nodes)
├── docs/
│   ├── results.txt             # Comprehensive experimental report & observations
│   └── simulated_results.txt   # Raw simulated benchmark matrix output
├── Makefile                    # Make build script
├── CMakeLists.txt              # CMake build configuration
├── results.txt                 # Root copy of experimental report
└── README.md                   # Project documentation & instructions
```

---

## 3. Implemented Graph Operations

1. **`connected_components(const Graph& g)`**:
   - Depth-First Search using an explicit `std::stack<int>`.
   - Iterative implementation prevents call-stack exhaustion on deep graphs with up to $10^6$ nodes.
   - Time Complexity: $O(V + E)$ | Space: $O(V)$.

2. **`one_cycle(const Graph& g)`**:
   - DFS with tri-state vertex coloring and predecessor tracking.
   - Ignores trivial backtracks to immediate parents; extracts simple cycles $\ge 3$ distinct vertices.
   - Returns empty vector `{}` if acyclic.
   - Time Complexity: $O(V + E)$ | Space: $O(V)$.

3. **`shortest_paths(const Graph& g, int source)`**:
   - Dijkstra's algorithm with min-heap priority queue (`std::priority_queue`).
   - Relaxations for unweighted edges (weight = 1).
   - Reconstructs path list $sp[v]$ from $v$ back to $source$ for all reachable vertices.
   - Excludes unreachable vertices.
   - Time Complexity: $O((V + E) \log V)$ | Space: $O(V + \sum \text{path lengths})$.

---

## 4. Simulated Graphs (All 6 Implemented)

1. **$n$-Cycle**: Single component, diameter $n/2$, unique cycle of length $n$.
2. **Complete Graph ($K_n$)**: Single component, unit shortest path, exponential cycles.
3. **Empty Graph**: $n$ components, 0 edges, acyclic, only source reachable.
4. **Binary Heap**: Single component (tree), short paths $O(\log n)$, acyclic.
5. **Truncated Heap**: $n - 1 - 2m$ edges, $m + 1$ components, short paths, acyclic.
6. **Equivalence Mod $k$**: $k$ components (disjoint cliques of size $\lceil n/k \rceil$).

---

## 5. Bonus: Real-Life Graph ($> 1,000,000$ Nodes)

* **Dataset:** Stanford SNAP California Road Network (`roadNet-CA`).
* **Scale:** 1,965,206 nodes, 2,766,607 undirected edges.
* **Three Adjacency Criteria:**
  - **Criterion 1 (Full Road Network):** All raw edges, normalized to $[0, V-1]$, self-loops removed, bidirectional.
    * 2,638 components in 69.2 ms (giant component of 1,957,027 nodes).
    * Cycle of length 8 in 3.02 ms.
    * Dijkstra shortest paths across 1.957M nodes in 2.66 s.
  - **Criterion 2 (Core Arterial Network):** Retains edges where both endpoints have degree $\ge 2$. Strips 321,213 dead-end cul-de-sacs.
    * 321,426 components in 125.1 ms.
  - **Criterion 3 (Modular Regional Partitioning):** Retains edges where $u \pmod 4 == v \pmod 4$. Partitions graph into 4 regional networks.
    * 1,396,622 components in 264.0 ms, max component size 17 nodes.

---

## 6. How to Build and Run

### Prerequisites
* C++17 compatible compiler (GCC / MinGW-w64 or Clang).

### Compilation via Makefile
```bash
# Build both simulated and real-graph benchmarks
make

# Or run individual executables:
./simulated_test
./run_realgraph_make
```

### Direct Compilation with g++
```bash
# Compile simulated test driver:
g++ -std=c++17 -O3 -Iinclude src/graph.cpp src/graph_operations.cpp src/graph_simulator.cpp src/simulated_test.cpp -o simulated_test -lpsapi

# Compile real-graph bonus driver:
g++ -std=c++17 -O3 -Iinclude src/graph.cpp src/graph_operations.cpp src/realgraph_make.cpp src/run_realgraph_make.cpp -o run_realgraph_make -lpsapi
```

---

## 7. Submission Archive

Per assignment specification:
* Archive filename: `Karimikonda Harsha assignment1.zip`
* Contains source files, drivers, results report, Makefile, and README.
