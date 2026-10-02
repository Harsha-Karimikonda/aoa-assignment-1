#include "graph_operations.h"
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>

std::vector<std::vector<int>> connected_components(const Graph& g) {
    const int n = g.get_num_vertices();
    std::vector<bool> visited(n, false);
    std::vector<std::vector<int>> components;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            std::vector<int> component;
            std::stack<int> s;

            s.push(i);
            visited[i] = true;

            while (!s.empty()) {
                int u = s.top();
                s.pop();
                component.push_back(u);

                const auto& neighbors = g.get_neighbors(u);
                for (int v : neighbors) {
                    if (!visited[v]) {
                        visited[v] = true;
                        s.push(v);
                    }
                }
            }
            components.push_back(std::move(component));
        }
    }
    return components;
}

std::vector<int> one_cycle(const Graph& g) {
    const int n = g.get_num_vertices();
    if (n < 3) {
        return {}; // No cycle with >= 3 distinct vertices can exist with < 3 nodes
    }

    // 0 = unvisited, 1 = visiting (on active DFS stack), 2 = visited and fully explored
    std::vector<int> state(n, 0);
    std::vector<int> parent(n, -1);

    // Iterative DFS simulating call stack with (vertex, neighbor_index)
    for (int start = 0; start < n; ++start) {
        if (state[start] != 0) continue;

        std::stack<std::pair<int, size_t>> s;
        s.push({start, 0});
        state[start] = 1;
        parent[start] = -1;

        while (!s.empty()) {
            int u = s.top().first;
            size_t& idx = s.top().second;
            const auto& neighbors = g.get_neighbors(u);

            if (idx < neighbors.size()) {
                int v = neighbors[idx++];

                if (v == parent[u]) {
                    // Trivial undirected backtrack along the edge we came from
                    continue;
                }

                if (state[v] == 1) {
                    // Back-edge found from u to ancestor v!
                    // Reconstruct cycle from u up to v via parent pointers
                    std::vector<int> cycle;
                    cycle.push_back(v);

                    std::vector<int> path_up;
                    int curr = u;
                    while (curr != v && curr != -1) {
                        path_up.push_back(curr);
                        curr = parent[curr];
                    }

                    // Cycle must have at least 3 distinct vertices: v, and at least 2 in path_up
                    if (curr == v && path_up.size() >= 2) {
                        for (auto it = path_up.rbegin(); it != path_up.rend(); ++it) {
                            cycle.push_back(*it);
                        }
                        cycle.push_back(v);
                        return cycle;
                    }
                } else if (state[v] == 0) {
                    // Forward edge to an unvisited vertex
                    state[v] = 1;
                    parent[v] = u;
                    s.push({v, 0});
                }
            } else {
                // Done exploring all neighbors of vertex u
                state[u] = 2;
                s.pop();
            }
        }
    }

    return {}; // No cycle found
}

std::map<int, std::vector<int>> shortest_paths(const Graph& g, int source) {
    std::map<int, std::vector<int>> sp;
    const int n = g.get_num_vertices();

    if (source < 0 || source >= n) {
        return sp;
    }

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    std::vector<int> pred(n, -1);

    // Min-priority queue storing (distance, vertex)
    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

    dist[source] = 0;
    pred[source] = source;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) {
            continue;
        }

        const auto& neighbors = g.get_neighbors(u);
        for (int v : neighbors) {
            // Unweighted graph: edge weight is intrinsically 1
            const int weight = 1;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pred[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Build the shortest path map
    // For each reachable vertex v, sp[v] contains the vertices from v back to source
    for (int v = 0; v < n; ++v) {
        if (dist[v] != INF) {
            std::vector<int> path;
            int curr = v;
            path.push_back(curr);

            while (curr != source) {
                curr = pred[curr];
                path.push_back(curr);
            }
            sp[v] = std::move(path);
        }
    }

    return sp;
}
