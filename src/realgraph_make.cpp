#include "realgraph_make.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

// Fast integer parsing helper
static inline bool parse_edge_line(const std::string& line, int& u, int& v) {
    if (line.empty() || line[0] == '#') {
        return false;
    }
    const char* p = line.c_str();
    while (*p && (*p == ' ' || *p == '\t')) ++p;
    if (!*p) return false;

    u = 0;
    while (*p >= '0' && *p <= '9') {
        u = u * 10 + (*p - '0');
        ++p;
    }

    while (*p && (*p == ' ' || *p == '\t')) ++p;
    if (!*p) return false;

    v = 0;
    while (*p >= '0' && *p <= '9') {
        v = v * 10 + (*p - '0');
        ++p;
    }
    return true;
}

// Pass 1: Reads raw edges and maps raw node IDs to 0-based contiguous indices [0, V-1]
struct RawEdge {
    int u;
    int v;
};

static void read_and_remap_edges(const std::string& filepath,
                                 std::vector<RawEdge>& raw_edges,
                                 std::unordered_map<int, int>& id_map) {
    std::ifstream fin(filepath);
    if (!fin.is_open()) {
        throw std::runtime_error("Could not open real-graph file: " + filepath);
    }

    std::string line;
    raw_edges.reserve(3000000); // Pre-reserve capacity for SNAP road network
    int next_id = 0;

    int raw_u, raw_v;
    while (std::getline(fin, line)) {
        if (!parse_edge_line(line, raw_u, raw_v)) {
            continue;
        }
        if (raw_u == raw_v) {
            continue; // Skip self-loops
        }

        auto it_u = id_map.find(raw_u);
        int mapped_u;
        if (it_u == id_map.end()) {
            mapped_u = next_id++;
            id_map[raw_u] = mapped_u;
        } else {
            mapped_u = it_u->second;
        }

        auto it_v = id_map.find(raw_v);
        int mapped_v;
        if (it_v == id_map.end()) {
            mapped_v = next_id++;
            id_map[raw_v] = mapped_v;
        } else {
            mapped_v = it_v->second;
        }

        raw_edges.push_back({mapped_u, mapped_v});
    }
    fin.close();
}

/**
 * Criterion 1: Full Baseline Road Network.
 * Reads all edges, maps vertices to 0..V-1, and adds undirected edges.
 */
Graph load_realgraph_criterion1(const std::string& filepath) {
    std::vector<RawEdge> raw_edges;
    std::unordered_map<int, int> id_map;
    read_and_remap_edges(filepath, raw_edges, id_map);

    int num_vertices = static_cast<int>(id_map.size());
    Graph g(num_vertices);

    // Track existing edges to avoid duplicate insertions if raw data has duplicates
    for (const auto& e : raw_edges) {
        if (e.u < e.v) { // Only insert one direction to avoid double-adding before deduplication
            g.add_edge(e.u, e.v);
        }
    }
    return g;
}

/**
 * Criterion 2: Arterial Transit Core (Degree-Threshold Filtering).
 * 
 * Comments:
 * In urban transport networks, nodes with degree 1 are dead ends / cul-de-sacs.
 * We calculate the initial degree of every vertex and retain only edges where
 * BOTH endpoints have degree >= min_degree. This eliminates peripheral branches
 * and leaves the primary arterial corridors.
 */
Graph load_realgraph_criterion2(const std::string& filepath, int min_degree) {
    std::vector<RawEdge> raw_edges;
    std::unordered_map<int, int> id_map;
    read_and_remap_edges(filepath, raw_edges, id_map);

    int num_vertices = static_cast<int>(id_map.size());
    std::vector<int> degree(num_vertices, 0);

    for (const auto& e : raw_edges) {
        if (e.u < e.v) {
            degree[e.u]++;
            degree[e.v]++;
        }
    }

    Graph g(num_vertices);
    for (const auto& e : raw_edges) {
        if (e.u < e.v) {
            if (degree[e.u] >= min_degree && degree[e.v] >= min_degree) {
                g.add_edge(e.u, e.v);
            }
        }
    }
    return g;
}

/**
 * Criterion 3: Modular Regional Partitioning (Modular Affinity).
 * 
 * Comments:
 * Simulates regional jurisdictional / zone partitioning where connections
 * are restricted to nodes sharing the same modular identity (u % mod_k == v % mod_k).
 * Edges spanning across different partitions are filtered out, partitioning
 * the large graph into mod_k distinct regional networks.
 */
Graph load_realgraph_criterion3(const std::string& filepath, int mod_k) {
    if (mod_k <= 0) mod_k = 2;

    std::vector<RawEdge> raw_edges;
    std::unordered_map<int, int> id_map;
    read_and_remap_edges(filepath, raw_edges, id_map);

    int num_vertices = static_cast<int>(id_map.size());
    Graph g(num_vertices);

    for (const auto& e : raw_edges) {
        if (e.u < e.v) {
            if ((e.u % mod_k) == (e.v % mod_k)) {
                g.add_edge(e.u, e.v);
            }
        }
    }
    return g;
}
