#ifndef GRAPH_SIMULATOR_H
#define GRAPH_SIMULATOR_H

#include "graph.h"

/**
 * @brief Generates an n-cycle graph.
 * 
 * Vertices are integers from 0 through n - 1.
 * Vertices u and v are connected by an edge if u - v = +/- 1 or u - v = +/- (n - 1).
 * Properties: 1 connected component, max shortest path <= n/2, unique cycle of length n.
 * 
 * @param n Number of vertices (must be >= 3).
 * @return Graph An n-cycle graph.
 */
Graph generate_n_cycle(int n);

/**
 * @brief Generates a complete graph Kn on n vertices.
 * 
 * Vertices are integers from 0 through n - 1.
 * Every pair of distinct vertices forms an edge.
 * Properties: 1 connected component, all shortest paths have length 1, many cycles.
 * 
 * @param n Number of vertices.
 * @return Graph A complete graph Kn.
 */
Graph generate_complete_graph(int n);

/**
 * @brief Generates an empty graph on n vertices.
 * 
 * Vertices are integers from 0 through n - 1 with no edges.
 * Properties: n connected components, no paths, no cycles.
 * 
 * @param n Number of vertices.
 * @return Graph An empty graph.
 */
Graph generate_empty_graph(int n);

/**
 * @brief Generates a binary heap graph on n vertices.
 * 
 * Vertices are integers from 0 through n - 1.
 * The neighbors of vertex v are (v - 1)/2, 2v + 1, and 2v + 2, within [0, n - 1].
 * Properties: 1 connected component (tree), short paths O(log n), acyclic (0 cycles).
 * 
 * @param n Number of vertices.
 * @return Graph A heap graph.
 */
Graph generate_heap(int n);

/**
 * @brief Generates a truncated heap graph on vertices m through n - 1.
 * 
 * Vertices are integers from m through n - 1, re-indexed to 0 through (n - m - 1).
 * The edge relationship is the heap parent-child relationship restricted to [m, n - 1].
 * Properties: n - 1 - 2m edges, m + 1 connected components, acyclic, short paths.
 * 
 * @param m Start vertex offset (must satisfy 0 <= 2*m < n - 1).
 * @param n Total heap upper bound.
 * @return Graph A truncated heap graph.
 */
Graph generate_truncated_heap(int m, int n);

/**
 * @brief Generates an equivalence mod k graph on n vertices.
 * 
 * Vertices are integers from 0 to n - 1.
 * Vertices u and v are connected if (u - v) is evenly divisible by k.
 * Properties: k connected components, each component is a complete graph (clique).
 * 
 * @param n Number of vertices.
 * @param k Modulo divisor (1 <= k <= n).
 * @return Graph An equivalence mod k graph.
 */
Graph generate_equivalence_mod_k(int n, int k);

#endif // GRAPH_SIMULATOR_H
