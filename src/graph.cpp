#include "graph.h"
#include <algorithm>

Graph::Graph(int n) : num_vertices(0), num_edges(0) {
    if (n > 0) {
        resize(n);
    }
}

void Graph::resize(int n) {
    if (n < 0) {
        throw std::invalid_argument("Number of vertices cannot be negative.");
    }
    num_vertices = n;
    adj.clear();
    adj.resize(n);
    num_edges = 0;
}

void Graph::add_edge(int u, int v) {
    if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
        throw std::out_of_range("Vertex index out of range in add_edge.");
    }
    if (u == v) {
        // Prevent self-loops in simple undirected graph
        return;
    }
    adj[u].push_back(v);
    adj[v].push_back(u);
    num_edges++;
}

int Graph::get_num_vertices() const {
    return num_vertices;
}

long long Graph::get_num_edges() const {
    return num_edges;
}

const std::vector<int>& Graph::get_neighbors(int u) const {
    if (u < 0 || u >= num_vertices) {
        throw std::out_of_range("Vertex index out of range in get_neighbors.");
    }
    return adj[u];
}

int Graph::get_degree(int u) const {
    if (u < 0 || u >= num_vertices) {
        throw std::out_of_range("Vertex index out of range in get_degree.");
    }
    return static_cast<int>(adj[u].size());
}

bool Graph::has_edge(int u, int v) const {
    if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
        return false;
    }
    const auto& neighbors = adj[u];
    return std::find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
}

void Graph::clear() {
    adj.clear();
    num_vertices = 0;
    num_edges = 0;
}
