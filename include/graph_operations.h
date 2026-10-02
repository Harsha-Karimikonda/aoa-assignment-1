#ifndef GRAPH_OPERATIONS_H
#define GRAPH_OPERATIONS_H

#include "graph.h"
#include <vector>
#include <map>

/**
 * @brief Computes all connected components of an undirected graph using Depth-First Search (DFS).
 * 
 * An iterative stack-based DFS is utilized to ensure stack safety on graphs with up to
 * millions of nodes without encountering call-stack overflow.
 * 
 * @param g The input graph.
 * @return std::vector<std::vector<int>> List of connected components, where each component is a list of vertices.
 */
std::vector<std::vector<int>> connected_components(const Graph& g);

/**
 * @brief Finds a cycle in the graph using Depth-First Search (DFS).
 * 
 * In an undirected graph, traversing (u, v) and immediately back to (v, u) is a trivial
 * 2-edge backtrack, not a cycle. This function detects back-edges to visited ancestors
 * (excluding the immediate parent), extracting a simple cycle containing 3 or more distinct vertices.
 * 
 * @param g The input graph.
 * @return std::vector<int> A cycle represented as a list of vertices starting and ending with the same
 *         vertex (e.g., [v0, v1, v2, ..., v0], length >= 4), or an empty vector if the graph is acyclic.
 */
std::vector<int> one_cycle(const Graph& g);

/**
 * @brief Computes shortest paths from a given source node using Dijkstra's algorithm.
 * 
 * While the graph is unweighted (edge weights intrinsically 1), standard Dijkstra's
 * algorithm with a min-priority queue is executed.
 * 
 * @param g The input graph.
 * @param source The starting source vertex.
 * @return std::map<int, std::vector<int>> A map where sp[v] contains the sequence of vertices on
 *         the shortest path from v back to source (including v and source). For source itself,
 *         sp[source] = {source}. If v is not reachable from source, v does not appear in the map.
 */
std::map<int, std::vector<int>> shortest_paths(const Graph& g, int source);

#endif // GRAPH_OPERATIONS_H
