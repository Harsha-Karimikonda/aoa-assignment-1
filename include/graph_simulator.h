#ifndef GRAPH_SIMULATOR_H
#define GRAPH_SIMULATOR_H

#include "graph_operations.h"

// 1. An n-cycle: vertices 0..n-1, edge between (i, (i+1)%n).
Graph generate_n_cycle(int n);

// 2. Complete graph Kn: every pair of distinct vertices forms an edge.
Graph generate_complete_graph(int n);

// 3. Binary heap: neighbors of v are (v-1)/2, 2v+1, and 2v+2.
Graph generate_heap(int n);

// 4. Empty graph on n vertices (0 edges).
Graph generate_empty_graph(int n);

// 5. Truncated heap: vertices m..n-1 with heap relationships.
Graph generate_truncated_heap(int m, int n);

// 6. Equivalence mod k: edge between u and v if (u - v) % k == 0.
Graph generate_equivalence_mod_k(int n, int k);

#endif // GRAPH_SIMULATOR_H
