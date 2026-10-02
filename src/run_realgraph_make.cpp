#include "graph.h"
#include "graph_operations.h"
#include "realgraph_make.h"
#include "benchmark_utils.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void print_header(const std::string& title) {
    std::cout << "\n=======================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "=======================================================================\n";
}

void analyze_criterion(const std::string& name, const Graph& g, int source_node, std::ostream& out) {
    out << "\n--- Analyzing: " << name << " ---\n";
    out << "  Total Vertices : " << g.get_num_vertices() << "\n";
    out << "  Total Edges    : " << g.get_num_edges() << "\n";

    Timer timer;

    // 1. Connected Components
    timer.reset();
    auto comps = connected_components(g);
    double cc_time = timer.elapsed_ms();
    out << "  Connected Components : " << comps.size() << " found in " 
        << std::fixed << std::setprecision(2) << cc_time << " ms\n";

    // Component size distribution
    size_t max_comp_size = 0;
    size_t singletons = 0;
    for (const auto& comp : comps) {
        if (comp.size() > max_comp_size) max_comp_size = comp.size();
        if (comp.size() == 1) singletons++;
    }
    out << "    Largest Component Size: " << max_comp_size << " nodes\n";
    out << "    Isolated Nodes (size 1): " << singletons << "\n";

    // 2. One Cycle
    timer.reset();
    auto cycle = one_cycle(g);
    double cycle_time = timer.elapsed_ms();
    out << "  Cycle Detection      : ";
    if (cycle.empty()) {
        out << "Acyclic (no cycles) in " << cycle_time << " ms\n";
    } else {
        out << "Cycle found of length " << cycle.size() << " in " << cycle_time << " ms\n";
        out << "    Sample vertices on cycle: ";
        size_t show_count = std::min(cycle.size(), size_t(10));
        for (size_t i = 0; i < show_count; ++i) {
            out << cycle[i] << (i + 1 < show_count ? " -> " : "");
        }
        if (cycle.size() > 10) out << " ... -> " << cycle.back();
        out << "\n";
    }

    // 3. Shortest Paths
    // Find an appropriate component for Dijkstra demonstration
    // If graph has components of moderate size (< 20,000), use one to show complete map.
    // If largest component is huge, pick a source and run Dijkstra.
    out << "  Shortest Paths (Dijkstra from source " << source_node << "):\n";
    timer.reset();
    auto sp = shortest_paths(g, source_node);
    double sp_time = timer.elapsed_ms();
    out << "    Computed in " << sp_time << " ms. Reachable vertices: " << sp.size() << "\n";

    // Print sample shortest paths
    size_t printed = 0;
    for (const auto& [target, path] : sp) {
        if (target != source_node && printed < 5) {
            out << "    Path from " << target << " back to " << source_node << " (hops: " << path.size() - 1 << "): ";
            size_t p_show = std::min(path.size(), size_t(8));
            for (size_t i = 0; i < p_show; ++i) {
                out << path[i] << (i + 1 < p_show ? " -> " : "");
            }
            if (path.size() > 8) out << " ... -> " << path.back();
            out << "\n";
            printed++;
        }
    }

    out << "  Peak Memory Usage    : " << std::fixed << std::setprecision(2) 
        << get_peak_memory_mb() << " MB\n";
}

int main(int argc, char* argv[]) {
    std::string filepath = "data/roadNet-CA.txt";
    if (argc > 1) {
        filepath = argv[1];
    }

    print_header("REAL-WORLD LARGE GRAPH BENCHMARK (1,000,000+ NODES)");
    std::cout << "Dataset: Stanford SNAP California Road Network (roadNet-CA)\n";
    std::cout << "File   : " << filepath << "\n\n";

    Timer load_timer;

    // =========================================================================
    // CRITERION 1: Full Baseline Road Network
    // =========================================================================
    std::cout << "Loading Real Graph under Criterion 1 (Full Road Network)...\n";
    load_timer.reset();
    Graph g1 = load_realgraph_criterion1(filepath);
    double load1_ms = load_timer.elapsed_ms();
    std::cout << "Loaded in " << load1_ms << " ms. Peak Memory: " << get_peak_memory_mb() << " MB\n";
    
    // Pick a source node with some neighbors
    int source1 = 0;
    while (source1 < g1.get_num_vertices() && g1.get_degree(source1) == 0) source1++;
    analyze_criterion("Criterion 1: Full Baseline Road Network", g1, source1, std::cout);

    // =========================================================================
    // CRITERION 2: Arterial Transit Core (Degree >= 2)
    // =========================================================================
    std::cout << "\nLoading Real Graph under Criterion 2 (Degree >= 2 Arterial Core)...\n";
    load_timer.reset();
    Graph g2 = load_realgraph_criterion2(filepath, 2);
    double load2_ms = load_timer.elapsed_ms();
    std::cout << "Loaded in " << load2_ms << " ms. Peak Memory: " << get_peak_memory_mb() << " MB\n";

    int source2 = source1;
    while (source2 < g2.get_num_vertices() && g2.get_degree(source2) == 0) source2++;
    analyze_criterion("Criterion 2: Core Arterial Network (Degree >= 2)", g2, source2, std::cout);

    // =========================================================================
    // CRITERION 3: Modular Regional Partitioning (u % 4 == v % 4)
    // =========================================================================
    std::cout << "\nLoading Real Graph under Criterion 3 (Modular Partition k = 4)...\n";
    load_timer.reset();
    Graph g3 = load_realgraph_criterion3(filepath, 4);
    double load3_ms = load_timer.elapsed_ms();
    std::cout << "Loaded in " << load3_ms << " ms. Peak Memory: " << get_peak_memory_mb() << " MB\n";

    int source3 = source1;
    while (source3 < g3.get_num_vertices() && g3.get_degree(source3) == 0) source3++;
    analyze_criterion("Criterion 3: Modular Partitioning (k = 4)", g3, source3, std::cout);

    std::cout << "\nReal-life graph operations completed successfully.\n";
    return 0;
}
