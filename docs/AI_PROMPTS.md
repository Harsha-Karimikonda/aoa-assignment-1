# AI Collaboration Log & Prompts Record

**Course:** Analysis of Algorithms - Assignment 1: Graph Algorithms  
**Student Name:** Harsha Karimikonda  
**AI Assistant:** Google Antigravity (Gemini)  
**Date:** Fall 2026  

---

## Overview & Academic Integrity Statement

This document provides a comprehensive record of the iterative AI prompts and collaborative technical discussions conducted while architecting, implementing, benchmarking, and documenting this assignment. The AI was utilized as an interactive pair-programming partner to explore design trade-offs, verify theoretical invariants, implement memory-efficient algorithms, and resolve platform compatibility issues. All algorithmic decisions, mathematical proofs, and architectural structures were driven, guided, and validated by the author.

---

## Phase 1: Problem Formulation & Architectural Decisions

### Prompt 1.1: Graph Model Selection (Directed vs. Undirected)
> *"I'm starting Assignment 1 for Analysis of Algorithms. The prompt requires choosing either a directed or undirected graph model and providing a justification. Looking at the 6 simulated graph families (cycle, complete, heap, truncated heap, empty, equiv mod k) and the road network bonus, which model makes the most theoretical sense, and how does that choice impact cycle detection?"*

**Collaboration & Technical Outcome:**
* Evaluated that all six simulated graph families represent symmetric, bidirectional relationships.
* Determined that in an undirected graph, simple cycle detection must ignore trivial parent backtracks ($u \to v \to u$) and identify back-edges to ancestors with $\ge 3$ distinct vertices.
* Concluded that an **Undirected Graph** is the most mathematically consistent model across all required graph families and physical transportation networks.

### Prompt 1.2: Core Data Structure & Memory Bounds
> *"We cannot use external libraries like Boost Graph Library. What is the most optimal standard C++ container structure for storing graphs with up to 2 million nodes? Let's compare an adjacency matrix, pointer-based linked list adjacency, and a contiguous vector of vectors."*

**Collaboration & Technical Outcome:**
* **Adjacency Matrix:** Ruled out immediately ($O(V^2)$ memory would require $\approx 4\text{ TB}$ for 2M nodes).
* **Linked-List Adjacency:** Ruled out due to 8-byte pointer overhead and memory fragmentation.
* **Contiguous Adjacency List (`std::vector<std::vector<int>>`):** Selected for $O(V + 2E)$ compact memory footprint, $O(1)$ vertex lookup, and cache-line locality during neighbor iteration.

---

## Phase 2: Core Algorithm Design & Implementation

### Prompt 2.1: Stack-Safe Connected Components via Iterative DFS
> *"For `connected_components()`, a standard recursive DFS will cause a call-stack overflow when running on deep linear graphs or the 1.96-million node road network. Let's design an iterative DFS implementation using an explicit `std::stack<int>`. How should we structure the visited tracking to ensure $O(V + E)$ time and $O(V)$ space?"*

**Collaboration & Technical Outcome:**
* Implemented iterative DFS using an explicit heap-allocated `std::stack<int>` and `std::vector<bool> visited`.
* Marking vertices as visited upon push prevented redundant stack insertions.
* Verified zero call-stack overflow risk on graphs with millions of vertices.

### Prompt 2.2: Cycle Detection with Undirected Predecessor Tracking
> *"For `one_cycle()`, the assignment requires returning a cycle of three or more vertices that starts and ends with the same vertex, or an empty list if acyclic. In an undirected graph, how do we use DFS with tri-state coloring and predecessor tracking to detect back-edges while ignoring the immediate parent backtrack?"*

**Collaboration & Technical Outcome:**
* Designed tri-state DFS vertex coloring:
  * `state = 0`: unvisited
  * `state = 1`: currently on active DFS stack (ancestor)
  * `state = 2`: fully explored
* Explicitly checked `if (v == parent[u]) continue;` to filter out immediate undirected backtracks.
* When discovering a back-edge to an ancestor with `state[v] == 1`, traced parent pointers from $u$ back to $v$ to reconstruct `cycle = [v, ..., u, v]` containing $\ge 3$ distinct vertices ($\ge 4$ elements).

### Prompt 2.3: Dijkstra Shortest Paths on Unweighted Graphs
> *"The assignment specifies implementing `shortest_paths()` using Dijkstra's algorithm with unit edge weights, returning `std::map<int, std::vector<int>>` where `sp[v]` contains the vertices on a path from $v$ back to `source`. Let's implement this with a min-priority queue and discuss the path reconstruction logic."*

**Collaboration & Technical Outcome:**
* Implemented Dijkstra using `std::priority_queue<std::pair<int, int>, ..., std::greater<>>`.
* Maintained a predecessor array `pred[v]`.
* Reconstructed paths backwards from $v$ to `source`, ensuring `sp[source] = {source}` and excluding unreachable vertices ($dist[v] == \infty$).

---

## Phase 3: Simulated Graph Generators & Invariant Validation

### Prompt 3.1: Formulating All 6 Simulated Graph Families
> *"Let's implement all 6 simulated graph families from the prompt: n-cycle, complete graph $K_n$, binary heap, truncated heap ($m \dots n-1$), empty graph, and equivalence mod $k$. Can you check my index math for the truncated heap to make sure it matches the theoretical formulas $n - 1 - 2m$ edges and $m + 1$ components?"*

**Collaboration & Technical Outcome:**
* Implemented generators in `graph_simulator.cpp`.
* Truncated heap: normalized vertex indices $m \dots n-1$ to $0 \dots n-m-1$ and verified both child indices $2v+1$ and $2v+2$ lie within range $[m, n-1]$.
* Verified that truncated heap produced exactly $m+1$ components and $N - 1 - 2m$ edges.
* Equivalence mod $k$: partitioned vertices into $k$ disjoint complete cliques, producing exactly $k$ components.

### Prompt 3.2: Automated Correctness & Scaling Benchmark Driver
> *"Let's build `simulated_test.cpp`. First, add automated correctness assertions to verify cycle length, component count, and hop counts on small instances (6-cycle, $K_5$, heap, truncated heap, mod 3). Then, create a scaling loop varying $N$ to record runtime and memory across all 6 families."*

**Collaboration & Technical Outcome:**
* Built automated assertion tests matching all theoretical properties.
* Structured benchmark loops testing scales up to $N = 200,000$.

---

## Phase 4: Bonus — Real-World Road Network Scaling (1.96M Nodes)

### Prompt 4.1: Dataset Selection & Preprocessing Pipeline
> *"For the bonus, I chose the Stanford SNAP California Road Network (`roadNet-CA`). It has 1,965,206 nodes and 2,766,607 edges. How should we write an efficient ingestion function in `realgraph_make.cpp` to parse this large file quickly and map arbitrary raw node IDs to 0-based contiguous indices?"*

**Collaboration & Technical Outcome:**
* Built a high-throughput parser skipping `#` headers and self-loops ($u == v$).
* Used an `unordered_map<int, int>` to map raw node identifiers to contiguous indices $[0, V-1]$.
* Deduped and ensured undirected bidirectional edges ($u < v$).

### Prompt 4.2: Designing Three Meaningful Adjacency Criteria
> *"The assignment states: 'At least three functions should be tried with a different adjacency criteria. The criteria should be mentioned as comments.' What are three domain-relevant criteria we can implement for a road network?"*

**Collaboration & Technical Outcome:**
* **Criterion 1 (Full Road Network):** Baseline topology containing all valid roadway connections.
* **Criterion 2 (Core Arterial Network):** Degree-threshold filtering ($\text{degree} \ge 2$), pruning suburban dead-end leaves (cul-de-sacs) to isolate the arterial backbone.
* **Criterion 3 (Modular Regional Partitioning):** Modular grouping ($u \pmod 4 == v \pmod 4$) simulating 4 distinct administrative transportation districts where cross-district highways are severed.
* Added detailed explanatory comments to `realgraph_make.h` and `realgraph_make.cpp`.

---

## Phase 5: Cross-Platform Profiling & Empirical Analysis

### Prompt 5.1: Cross-Platform Peak Memory Profiling
> *"I am testing across Windows and macOS. How can we implement `get_peak_memory_mb()` in `benchmark.h` so that it queries the OS kernel accurately on Windows (`GetProcessMemoryInfo`), macOS (`getrusage`), and Linux?"*

**Collaboration & Technical Outcome:**
* Implemented cross-platform preprocessor directives:
  * Windows: `GetProcessMemoryInfo` reading `PeakWorkingSetSize`.
  * macOS: `getrusage(RUSAGE_SELF, ...)` reading `ru_maxrss` (in bytes on macOS).
  * Linux: `ru_maxrss` (in kilobytes on Linux).

### Prompt 5.2: Analyzing Dijkstra Memory Scaling Discrepancy
> *"When looking at the benchmark numbers, why does Dijkstra shortest paths take $O(N^2)$ time and memory on the $n$-cycle, but runs in sub-linear time on binary heaps, even though both graphs have $O(N)$ edges?"*

**Collaboration & Technical Outcome:**
* Explained that on an $n$-cycle, the diameter is $n/2$, meaning average shortest path length is $n/4$. Storing all paths in `std::map<int, std::vector<int>>` consumes $O(N^2)$ integer storage.
* On binary heaps, tree depth is bounded by $\log_2 N \le 18$, so total path storage is only $O(N \log N)$ integers.
* Documented this empirical and theoretical finding in `results.txt`.

---

## Phase 6: Build Automation, Documentation & Packaging

### Prompt 6.1: Cross-Platform Makefile & Packaging
> *"Let's structure the repository into `include/`, `src/`, and `docs/`. Can you help write a Makefile that compiles with `-O3` on both macOS (Clang) and Windows (GCC with `-lpsapi`), and includes a `make package` rule that prepares the submission zip `Lastname Firstname assignment1.zip` with all required files?"*

**Collaboration & Technical Outcome:**
* Structured Makefile with platform detection (`ifeq ($(OS),Windows_NT)`).
* Added `make package` target generating `Karimikonda Harsha assignment1.zip` containing both organized directory structures and root aliases.
* Created clean `.gitignore` to prevent tracking binaries and large datasets.

---

## Summary of Learning & Takeaways
1. **Algorithm Engineering:** Implementing theoretical graph algorithms in pure C++ requires careful attention to recursion depth, cache locality, and container memory overhead.
2. **Empirical vs. Theoretical Bounds:** Algorithmic complexity is heavily influenced by representation details (such as the quadratic memory footprint of storing paths in high-diameter graphs).
3. **AI Pair Programming:** Using AI to brainstorm architectural trade-offs, sanity-check mathematical indexing, and assist with build automation accelerated implementation while keeping core problem-solving and rigorous verification firmly in the student's hands.
