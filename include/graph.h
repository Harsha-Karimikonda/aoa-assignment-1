#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cstddef>
#include <stdexcept>

/**
 * @brief Graph representation for undirected graphs.
 *
 * Design Decision & Data Structure Description:
 * ---------------------------------------------
 * 1. Graph Model: UNDIRECTED GRAPH.
 *    Selected because all simulated benchmark graphs (n-cycle, complete graph,
 *    empty graph, heap, truncated heap, equivalence mod k) and the real-world road
 *    network represent symmetric, bidirectional relationships.
 *
 * 2. Data Structure: Contiguous Adjacency List using std::vector<std::vector<int>>.
 *    - Memory efficiency: Space complexity is O(V + E), using contiguous arrays of integers.
 *      This completely eliminates the pointer and node-allocation overhead of linked lists (std::list).
 *    - Cache locality: Iterating over vertex neighbors touches sequential memory in the CPU cache,
 *      minimizing cache misses during DFS traversals and Dijkstra edge relaxations.
 *    - Lookup and traversal:
 *      * Vertex neighbor access: O(1) random access via adj[u].
 *      * Iteration over all neighbors of u: O(deg(u)).
 *      * Edge insertion: Amortized O(1).
 */
class Graph {
private:
    int num_vertices;
    long long num_edges;
    std::vector<std::vector<int>> adj;

public:
    /**
     * @brief Constructs a graph with a fixed number of vertices.
     * @param n Number of vertices (indexed 0 to n-1).
     */
    explicit Graph(int n = 0);

    /**
     * @brief Resizes the graph to accommodate n vertices.
     * @param n New vertex count.
     */
    void resize(int n);

    /**
     * @brief Adds an undirected edge between vertices u and v.
     * @param u Endpoint vertex 1.
     * @param v Endpoint vertex 2.
     */
    void add_edge(int u, int v);

    /**
     * @brief Returns the number of vertices in the graph.
     */
    int get_num_vertices() const;

    /**
     * @brief Returns the total number of undirected edges.
     */
    long long get_num_edges() const;

    /**
     * @brief Returns a reference to the adjacency list of vertex u.
     * @param u Vertex index.
     */
    const std::vector<int>& get_neighbors(int u) const;

    /**
     * @brief Returns the degree of vertex u.
     * @param u Vertex index.
     */
    int get_degree(int u) const;

    /**
     * @brief Checks if an edge exists between u and v.
     * @param u Endpoint vertex 1.
     * @param v Endpoint vertex 2.
     */
    bool has_edge(int u, int v) const;

    /**
     * @brief Clears all vertices and edges.
     */
    void clear();
};

#endif // GRAPH_H
