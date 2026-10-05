# AI Prompts Used for Assignment 1

**Course:** Analysis of Algorithms - Assignment 1 (Graph Algorithms)  
**Author:** Harsha Karimikonda  
**Tool:** Gemini / Antigravity AI Assistant  

Below is the chronological sequence of prompts and interactions used while working on this assignment.

---

### Prompt 1: Understanding the Assignment Description
> "Here is the assignment description for my algorithms class: [pasted full assignment prompt]. Can you break this down for me and explain what all the requirements, parts, and deliverables are?"

**Summary of Discussion:**
The assistant broke down the prompt into three main parts:
1. Implementing a custom graph data structure and the 3 core functions (`connected_components`, `one_cycle`, `shortest_paths`).
2. Generating at least 3 simulated graph types and benchmarking them.
3. Bonus: running the algorithms on a real-world graph with $\ge 1\text{M}$ nodes using 3 different adjacency criteria.
Deliverables included `results.txt`, source files, and a specific zip naming format.

---

### Prompt 2: Choosing C++ and the Graph Data Structure
> "The assignment allows either C++ or Java. I want to do this in C++ rather than Java. Let's decide on the graph model first: should we treat it as directed or undirected? Also, what basic C++ data structure should we use to store the graph so that it's fast and doesn't run out of memory on up to 2 million nodes?"

**Summary of Discussion:**
- Selected an **undirected graph** model because all 6 simulated graph families and road networks represent bidirectional connections.
- Selected an adjacency list using `std::vector<std::vector<int>>` inside a `Graph` struct. We ruled out an adjacency matrix because 2 million nodes would take terabytes of RAM.

---

### Prompt 3: Implementing the Three Core Functions
> "Let's implement the three required functions in `graph_operations.cpp`:
> 1. `connected_components`: Can we make this iterative with `std::stack` so deep recursion doesn't cause a stack overflow on large graphs?
> 2. `one_cycle`: How do we make sure DFS in an undirected graph doesn't immediately backtrack to the parent node and call it a cycle? It needs to return a cycle with at least 3 distinct vertices.
> 3. `shortest_paths`: Let's implement Dijkstra's algorithm for unweighted graphs (unit weight 1) using a priority queue, returning a map where each path traces from vertex `v` back to `source`."

**Summary of Discussion:**
Implemented all three functions in `src/graph_operations.cpp`:
- `connected_components` uses iterative DFS with `std::stack<int>` and a visited vector ($O(V + E)$).
- `one_cycle` uses DFS with tri-state coloring and parent tracking, ignoring immediate parent backtracks and returning `[v, ..., u, v]`.
- `shortest_paths` uses Dijkstra with `std::priority_queue` and path reconstruction from $v$ back to `source`.

---

### Prompt 4: Simulated Graph Generators & Test Suite
> "I also want you to write the testing files and generators. Can we implement all 6 simulated graph generators in `graph_simulator.cpp` (n-cycle, complete Kn, binary heap, truncated heap, empty, and equivalence mod k)? Then let's write `simulated_test.cpp` to run correctness checks on small graphs and scale up nodes to benchmark runtime and memory."

**Summary of Discussion:**
- Implemented all 6 graph generators in `src/graph_simulator.cpp`, ensuring the truncated heap edge and component counts matched the formulas.
- Created `src/simulated_test.cpp` which runs automated correctness assertions (6-cycle, K5, heap acyclicity, truncated heap components, mod 3) and prints a scaling benchmark table up to 200,000 nodes.

---

### Prompt 5: Bonus — Real-World Road Network (1.96M Nodes)
> "For the bonus, I'm using the California Road Network from Stanford SNAP (`roadNet-CA.txt`, ~1.96M nodes, 2.76M edges). How should we parse this file in `realgraph_make.cpp` to map arbitrary node IDs to contiguous 0..V-1 indices? Also, what are 3 meaningful adjacency criteria we can implement?"

**Summary of Discussion:**
- Wrote an efficient parser in `src/realgraph_make.cpp` using `unordered_map` to map raw IDs to contiguous indices `0..V-1`, filtering out self-loops and comments.
- Implemented 3 domain-relevant criteria:
  1. Full Baseline Road Network (all edges).
  2. Core Arterial Network (degree $\ge$ 2 filtering, stripping dead-end cul-de-sacs).
  3. Modular Regional Partitioning ($u \pmod 4 == v \pmod 4$, simulating 4 regional transportation districts).
- Wrote `src/run_realgraph_make.cpp` to evaluate all 3 criteria on the road network.

---

### Prompt 6: Cross-Platform Memory and CPU Profiling
> "How do we measure peak memory consumption and CPU runtime in C++ across both macOS and Windows without using external libraries?"

**Summary of Discussion:**
Created `include/benchmark.h` using `#ifdef` directives:
- Windows: `GetProcessMemoryInfo` for `PeakWorkingSetSize`.
- macOS: `getrusage(RUSAGE_SELF, ...)` using `ru_maxrss` (bytes).
- Linux: `getrusage` using `ru_maxrss` (KB).
- Used `std::chrono::high_resolution_clock` for millisecond-level CPU timing.

---

### Prompt 7: Build Automation, Report & Submission Packaging
> "Can we write a Makefile to build both binaries (`simulated_test` and `run_realgraph_make`) on macOS and Windows? Also, help me format `results.txt` with our benchmark tables, observations, and sample outputs, and add a rule to package everything into `Karimikonda Harsha assignment1.zip` as specified in the prompt."

**Summary of Discussion:**
- Created `Makefile` with clean build and `package` targets.
- Formatted `results.txt` with experimental data, memory/time scaling analysis, and sample test outputs.
- Configured packaging to produce `Karimikonda Harsha assignment1.zip` with all required files and directory structure.
