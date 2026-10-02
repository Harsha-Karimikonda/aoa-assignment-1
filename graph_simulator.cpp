#include "graph_simulator.h"

Graph generate_n_cycle(int n) {
    Graph g(n);
    for (int i = 0; i < n; ++i) g.add_edge(i, (i + 1) % n);
    return g;
}

Graph generate_complete_graph(int n) {
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) g.add_edge(i, j);
    }
    return g;
}

Graph generate_heap(int n) {
    Graph g(n);
    for (int v = 0; v < n; ++v) {
        if (2 * v + 1 < n) g.add_edge(v, 2 * v + 1);
        if (2 * v + 2 < n) g.add_edge(v, 2 * v + 2);
    }
    return g;
}

Graph generate_empty_graph(int n) {
    return Graph(n);
}

Graph generate_truncated_heap(int m, int n) {
    Graph g(n - m);
    for (int v = m; v < n; ++v) {
        int l = 2 * v + 1, r = 2 * v + 2;
        if (l >= m && l < n) g.add_edge(v - m, l - m);
        if (r >= m && r < n) g.add_edge(v - m, r - m);
    }
    return g;
}

Graph generate_equivalence_mod_k(int n, int k) {
    Graph g(n);
    for (int r = 0; r < k; ++r) {
        std::vector<int> group;
        for (int v = r; v < n; v += k) group.push_back(v);
        for (size_t i = 0; i < group.size(); ++i) {
            for (size_t j = i + 1; j < group.size(); ++j) g.add_edge(group[i], group[j]);
        }
    }
    return g;
}
