# AI Prompts & Collaboration Log

**Assignment:** Analysis of Algorithms - Assignment 1 (Graph Algorithms)  
**Author:** Harsha Karimikonda  
**Tool Used:** Gemini / Antigravity AI Assistant  

This document lists the prompts and discussions I had with the AI assistant while planning, implementing, testing, and packaging this assignment.

---

### 1. Project Planning & Data Structure

**Prompt:**
> "I am working on Assignment 1 for my algorithms class. We need to implement connected components, cycle detection, and shortest paths in C++ without external graph libraries. Should I treat the graphs as directed or undirected, and what data structure is best if we might test up to 2 million nodes?"

**Discussion & Decision:**
We decided on an **undirected graph** because all 6 simulated graphs and road networks represent symmetric relationships. For the data structure, we chose an adjacency list using `std::vector<std::vector<int>>` because an adjacency matrix would take terabytes for 2M nodes, and linked lists have too much pointer overhead and cache misses.

---

### 2. Implementing the 3 Graph Functions

**Prompt:**
> "For connected components, if I use recursive DFS it might crash with a stack overflow on large or deep graphs. Can we write an iterative DFS using `std::stack` instead?"

**Discussion & Decision:**
Implemented `connected_components` using an explicit heap-allocated `std::stack<int>` and a visited vector. This ensures it runs in $O(V + E)$ time without any risk of blowing the call stack on graphs with millions of vertices.

---

**Prompt:**
> "For `one_cycle`, the assignment asks for a cycle of at least 3 unique nodes starting and ending at the same node. In an undirected graph, how do I prevent DFS from just going back to the parent node and calling that a cycle?"

**Discussion & Decision:**
Tracked the parent of each vertex during DFS. If a neighbor is the parent, we skip it. If a neighbor is already in the current DFS path (an ancestor), we trace back through parent pointers to construct the cycle `[v, ..., u, v]` with $\ge 3$ distinct vertices.

---

**Prompt:**
> "The prompt requires Dijkstra's algorithm for shortest paths with unit edge weights, returning a map where `sp[v]` is the path from `v` back to `source`. Can we implement this with `std::priority_queue`?"

**Discussion & Decision:**
Implemented Dijkstra with a min-priority queue and a predecessor array. Reconstructed the path backwards from each reachable node to the source. Unreachable nodes are excluded from the map.

---

### 3. Simulated Graph Generators & Testing

**Prompt:**
> "Can you help me implement the 6 simulated graph generators in `graph_simulator.cpp`? Specifically, check my math for the truncated heap to make sure the indices match the formula in the prompt."

**Discussion & Decision:**
Implemented all 6 generators (n-cycle, complete Kn, binary heap, truncated heap, empty graph, and mod k). Verified that the truncated heap with $m = n/4$ produces $m + 1$ components and $n - 1 - 2m$ edges.

---

**Prompt:**
> "Let's write a driver program `simulated_test.cpp` that checks correctness assertions on small graphs first, and then runs benchmarks on larger sizes while printing runtime and peak memory."

**Discussion & Decision:**
Created `simulated_test.cpp` with assertion checks (6-cycle, K5, heap acyclicity, truncated heap components, mod 3) and a benchmark loop that prints a clean table for sizes up to 200,000 nodes.

---

### 4. Real-World Road Network Bonus

**Prompt:**
> "For the bonus, I downloaded the California road network dataset from Stanford SNAP (`roadNet-CA.txt`, ~1.96M nodes). How should we parse the text file efficiently and map the node IDs to contiguous numbers from 0 to V-1?"

**Discussion & Decision:**
Wrote `realgraph_make.cpp` using `std::ifstream` and `std::unordered_map` to parse raw node IDs into contiguous indices $0 \dots V-1$, skipping comments and self-loops.

---

**Prompt:**
> "The bonus asks for at least 3 different adjacency criteria with comments. What are three meaningful criteria we can test on this road dataset?"

**Discussion & Decision:**
Implemented:
1. Full baseline road network (all valid edges).
2. Core arterial network (degree $\ge$ 2 filtering to prune dead-end suburban cul-de-sacs).
3. Modular partition ($u \pmod 4 == v \pmod 4$ to simulate 4 regional transportation districts).

---

### 5. Benchmarking & Memory Profiling

**Prompt:**
> "How do I measure peak memory in C++ without external libraries on both macOS and Windows?"

**Discussion & Decision:**
Created `include/benchmark.h` using `#ifdef` preprocessor checks for `GetProcessMemoryInfo` on Windows and `getrusage` (`ru_maxrss`) on macOS/Linux.

---

**Prompt:**
> "Why does Dijkstra take significantly longer on the n-cycle (e.g., 5,000 nodes) compared to the binary heap, even though both have roughly the same number of edges?"

**Discussion & Decision:**
Noticed that the diameter of an n-cycle is $n/2$, meaning average shortest path length is $n/4$. Storing all paths back to source takes $O(N^2)$ memory. In a binary heap, tree depth is $O(\log N)$, so paths only take $O(N \log N)$ space. Documented this observation in `results.txt`.

---

### 6. Makefile & Submission Packaging

**Prompt:**
> "Can we write a clean Makefile that works on both macOS Clang and Windows GCC, and add a `make package` rule that creates `Karimikonda Harsha assignment1.zip` with all required files?"

**Discussion & Decision:**
Configured `Makefile` with cross-platform flags and a `package` target that stages and creates the submission zip requested by the prompt.
