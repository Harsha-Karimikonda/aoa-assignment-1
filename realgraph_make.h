#ifndef REALGRAPH_MAKE_H
#define REALGRAPH_MAKE_H

#include "graph_operations.h"
#include <string>

// Adjacency Criteria 1: Full baseline road network.
// Ingests all edges, maps node IDs to contiguous 0..V-1 indices, ensures undirected edges.
Graph load_realgraph_full(const std::string& filepath);

// Adjacency Criteria 2: Core arterial network (Degree-threshold filtering).
// Retains edges only if both endpoints have degree >= min_degree, stripping dead-end leaves.
Graph load_realgraph_core_degree(const std::string& filepath, int min_degree = 2);

// Adjacency Criteria 3: Modular regional partitioning.
// Retains edges only if u % mod_k == v % mod_k, partitioning the network into mod_k regional zones.
Graph load_realgraph_modular_partition(const std::string& filepath, int mod_k = 4);

#endif // REALGRAPH_MAKE_H
