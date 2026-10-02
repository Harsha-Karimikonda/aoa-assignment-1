#include "graph_operations.h"
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>

std::vector<std::vector<int>> connected_components(const Graph& g) {
    std::vector<bool> visited(g.V, false);
    std::vector<std::vector<int>> comps;

    for (int i = 0; i < g.V; ++i) {
        if (!visited[i]) {
            std::vector<int> comp;
            std::stack<int> s;
            s.push(i);
            visited[i] = true;

            while (!s.empty()) {
                int u = s.top();
                s.pop();
                comp.push_back(u);

                for (int v : g.adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        s.push(v);
                    }
                }
            }
            comps.push_back(std::move(comp));
        }
    }
    return comps;
}

std::vector<int> one_cycle(const Graph& g) {
    std::vector<int> state(g.V, 0); // 0 = unvisited, 1 = on current DFS stack, 2 = visited
    std::vector<int> parent(g.V, -1);

    for (int i = 0; i < g.V; ++i) {
        if (state[i] != 0) continue;

        std::stack<std::pair<int, size_t>> s;
        s.push({i, 0});
        state[i] = 1;

        while (!s.empty()) {
            int u = s.top().first;
            size_t& idx = s.top().second;

            if (idx < g.adj[u].size()) {
                int v = g.adj[u][idx++];
                if (v == parent[u]) continue; // Skip immediate parent backtrack

                if (state[v] == 1) { // Found back-edge to ancestor v!
                    std::vector<int> path_up;
                    for (int cur = u; cur != v && cur != -1; cur = parent[cur]) {
                        path_up.push_back(cur);
                    }
                    if (path_up.size() >= 2) {
                        std::vector<int> cycle = {v};
                        for (auto it = path_up.rbegin(); it != path_up.rend(); ++it) {
                            cycle.push_back(*it);
                        }
                        cycle.push_back(v);
                        return cycle; // Format: [v, ..., u, v] with >= 3 distinct vertices
                    }
                } else if (state[v] == 0) {
                    state[v] = 1;
                    parent[v] = u;
                    s.push({v, 0});
                }
            } else {
                state[u] = 2;
                s.pop();
            }
        }
    }
    return {}; // Acyclic
}

std::map<int, std::vector<int>> shortest_paths(const Graph& g, int source) {
    std::map<int, std::vector<int>> sp;
    if (source < 0 || source >= g.V) return sp;

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(g.V, INF), pred(g.V, -1);

    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

    dist[source] = 0;
    pred[source] = source;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;

        for (int v : g.adj[u]) {
            if (dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    for (int v = 0; v < g.V; ++v) {
        if (dist[v] != INF) {
            std::vector<int> path;
            for (int cur = v; ; cur = pred[cur]) {
                path.push_back(cur);
                if (cur == source) break;
            }
            sp[v] = std::move(path); // Path starts at v and ends at source
        }
    }
    return sp;
}
