#include "realgraph_make.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

struct Edge { int u, v; };

// Helper to read and map raw node IDs to contiguous indices [0, V-1]
static void read_edges(const std::string& path, std::vector<Edge>& edges, int& num_v) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::unordered_map<int, int> id_map;
    int next_id = 0, raw_u, raw_v;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        if (!(ss >> raw_u >> raw_v) || raw_u == raw_v) continue;

        if (!id_map.count(raw_u)) id_map[raw_u] = next_id++;
        if (!id_map.count(raw_v)) id_map[raw_v] = next_id++;
        edges.push_back({id_map[raw_u], id_map[raw_v]});
    }
    num_v = next_id;
}

// Criteria 1: Full baseline road network with all valid bidirectional edges.
Graph load_realgraph_full(const std::string& filepath) {
    std::vector<Edge> edges;
    int num_v = 0;
    read_edges(filepath, edges, num_v);
    Graph g(num_v);
    for (const auto& e : edges) {
        if (e.u < e.v) g.add_edge(e.u, e.v);
    }
    return g;
}

// Criteria 2: Core arterial network retaining edges where both endpoints have degree >= min_degree.
// Prunes residential cul-de-sacs and dead-ends (degree 1 vertices).
Graph load_realgraph_core_degree(const std::string& filepath, int min_degree) {
    std::vector<Edge> edges;
    int num_v = 0;
    read_edges(filepath, edges, num_v);
    std::vector<int> deg(num_v, 0);
    for (const auto& e : edges) {
        if (e.u < e.v) { deg[e.u]++; deg[e.v]++; }
    }
    Graph g(num_v);
    for (const auto& e : edges) {
        if (e.u < e.v && deg[e.u] >= min_degree && deg[e.v] >= min_degree) {
            g.add_edge(e.u, e.v);
        }
    }
    return g;
}

// Criteria 3: Regional modular partitioning where edges connect nodes with same residue (u%k == v%k).
// Segments the entire graph into mod_k regional subnetworks.
Graph load_realgraph_modular_partition(const std::string& filepath, int mod_k) {
    std::vector<Edge> edges;
    int num_v = 0;
    read_edges(filepath, edges, num_v);
    Graph g(num_v);
    for (const auto& e : edges) {
        if (e.u < e.v && (e.u % mod_k) == (e.v % mod_k)) {
            g.add_edge(e.u, e.v);
        }
    }
    return g;
}
