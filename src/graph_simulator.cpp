#include "graph_simulator.h"
#include <stdexcept>
#include <vector>

Graph generate_n_cycle(int n) {
    if (n < 3) {
        throw std::invalid_argument("An n-cycle requires at least 3 vertices.");
    }
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        g.add_edge(i, next);
    }
    return g;
}

Graph generate_complete_graph(int n) {
    if (n < 0) {
        throw std::invalid_argument("Number of vertices cannot be negative.");
    }
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            g.add_edge(i, j);
        }
    }
    return g;
}

Graph generate_empty_graph(int n) {
    if (n < 0) {
        throw std::invalid_argument("Number of vertices cannot be negative.");
    }
    Graph g(n);
    return g;
}

Graph generate_heap(int n) {
    if (n < 0) {
        throw std::invalid_argument("Number of vertices cannot be negative.");
    }
    Graph g(n);
    for (int v = 0; v < n; ++v) {
        int left = 2 * v + 1;
        int right = 2 * v + 2;
        if (left < n) {
            g.add_edge(v, left);
        }
        if (right < n) {
            g.add_edge(v, right);
        }
    }
    return g;
}

Graph generate_truncated_heap(int m, int n) {
    if (m < 0 || n <= m) {
        throw std::invalid_argument("Invalid bounds: require 0 <= m < n.");
    }
    int num_v = n - m;
    Graph g(num_v);

    // Vertices are original integers m to n - 1, indexed as (v - m) in [0, num_v - 1]
    for (int v = m; v < n; ++v) {
        int left = 2 * v + 1;
        int right = 2 * v + 2;
        if (left >= m && left < n) {
            g.add_edge(v - m, left - m);
        }
        if (right >= m && right < n) {
            g.add_edge(v - m, right - m);
        }
    }
    return g;
}

Graph generate_equivalence_mod_k(int n, int k) {
    if (k <= 0 || k > n) {
        throw std::invalid_argument("Require 1 <= k <= n for equivalence mod k.");
    }
    Graph g(n);

    // Group vertices by their residue class modulo k
    for (int r = 0; r < k; ++r) {
        std::vector<int> class_vertices;
        for (int v = r; v < n; v += k) {
            class_vertices.push_back(v);
        }
        // Connect every pair in the equivalence class (clique)
        for (size_t i = 0; i < class_vertices.size(); ++i) {
            for (size_t j = i + 1; j < class_vertices.size(); ++j) {
                g.add_edge(class_vertices[i], class_vertices[j]);
            }
        }
    }
    return g;
}
