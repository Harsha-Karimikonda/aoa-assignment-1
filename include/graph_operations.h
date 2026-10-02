#ifndef GRAPH_OPERATIONS_H
#define GRAPH_OPERATIONS_H

#include <vector>
#include <map>

// Model: UNDIRECTED GRAPH.
// Data Structure: Adjacency list using std::vector<std::vector<int>>.
// Provides O(V + E) space, O(1) vertex neighbor access, and cache-friendly iteration.
struct Graph {
    int V;
    std::vector<std::vector<int>> adj;

    Graph(int n = 0) : V(n), adj(n) {}

    void add_edge(int u, int v) {
        if (u != v && u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
};

// Returns connected components using DFS. Each component is a list of vertices.
std::vector<std::vector<int>> connected_components(const Graph& g);

// Returns a cycle of >= 3 distinct vertices starting and ending with the same vertex, or {} if acyclic.
std::vector<int> one_cycle(const Graph& g);

// Returns map of shortest paths from source using Dijkstra's algorithm.
// For reachable v, sp[v] contains the vertices from v back to source (including v and source).
std::map<int, std::vector<int>> shortest_paths(const Graph& g, int source);

#endif // GRAPH_OPERATIONS_H
