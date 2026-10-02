#include "graph_operations.h"
#include "realgraph_make.h"
#include "benchmark.h"
#include <iostream>
#include <iomanip>

void evaluate(const std::string& name, const Graph& g, int source) {
    using clock = std::chrono::high_resolution_clock;
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Vertices: " << g.V << "\n";

    // 1. Connected Components
    auto t0 = clock::now();
    auto comps = connected_components(g);
    double t_cc = std::chrono::duration<double, std::milli>(clock::now() - t0).count();
    std::cout << "Connected Components: " << comps.size() << " found in " << t_cc << " ms\n";

    // 2. Cycle Detection
    t0 = clock::now();
    auto cycle = one_cycle(g);
    double t_cyc = std::chrono::duration<double, std::milli>(clock::now() - t0).count();
    if (!cycle.empty()) {
        std::cout << "Cycle Found: length=" << cycle.size() << " in " << t_cyc << " ms (Sample: ";
        for (size_t i = 0; i < std::min(cycle.size(), size_t(6)); ++i) std::cout << cycle[i] << " ";
        std::cout << "...)\n";
    } else {
        std::cout << "Acyclic in " << t_cyc << " ms\n";
    }

    // 3. Shortest Paths
    t0 = clock::now();
    auto sp = shortest_paths(g, source);
    double t_sp = std::chrono::duration<double, std::milli>(clock::now() - t0).count();
    std::cout << "Dijkstra SP from source " << source << ": " << sp.size() << " reachable in " << t_sp << " ms\n";
    if (sp.count(source)) {
        for (const auto& [target, path] : sp) {
            if (target != source) {
                std::cout << "Sample path from " << target << " back to " << source << ": ";
                for (int v : path) std::cout << v << " ";
                std::cout << "\n";
                break;
            }
        }
    }

    std::cout << "Peak Memory: " << std::fixed << std::setprecision(2) << get_peak_memory_mb() << " MB\n";
}

int main(int argc, char* argv[]) {
    std::string path = (argc > 1) ? argv[1] : "data/roadNet-CA.txt";
    std::cout << "Real Graph Benchmark on: " << path << "\n";

    std::cout << "Loading Criterion 1 (Full Road Network)...\n";
    Graph g1 = load_realgraph_full(path);
    evaluate("Criterion 1: Full Road Network", g1, 0);

    std::cout << "\nLoading Criterion 2 (Arterial Core, Degree >= 2)...\n";
    Graph g2 = load_realgraph_core_degree(path, 2);
    evaluate("Criterion 2: Core Arterial Network", g2, 0);

    std::cout << "\nLoading Criterion 3 (Modular Partition, k = 4)...\n";
    Graph g3 = load_realgraph_modular_partition(path, 4);
    evaluate("Criterion 3: Modular Partitioning (k=4)", g3, 0);

    return 0;
}
