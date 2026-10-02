#include "graph.h"
#include "graph_operations.h"
#include "graph_simulator.h"
#include "benchmark_utils.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

void print_separator(char c = '-', int width = 80) {
    std::cout << std::string(width, c) << "\n";
}

void run_correctness_tests() {
    std::cout << "\n=======================================================\n";
    std::cout << "           RUNNING CORRECTNESS VALIDATION TESTS        \n";
    std::cout << "=======================================================\n\n";

    // 1. n-cycle test (n = 6)
    {
        std::cout << "[Test 1] 6-Cycle Graph:\n";
        Graph g = generate_n_cycle(6);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);
        auto sp = shortest_paths(g, 0);

        std::cout << "  Vertices: " << g.get_num_vertices() << ", Edges: " << g.get_num_edges() << "\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 1)\n";
        std::cout << "  Cycle detected: ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << cycle[i] << (i + 1 < cycle.size() ? " -> " : "");
        }
        std::cout << " (Length: " << cycle.size() << ")\n";

        std::cout << "  Sample Shortest Path (0 to 3, path from 3 back to 0): ";
        if (sp.count(3)) {
            for (size_t i = 0; i < sp[3].size(); ++i) {
                std::cout << sp[3][i] << (i + 1 < sp[3].size() ? " -> " : "");
            }
        }
        std::cout << "\n  Path from source 0 back to 0: ";
        if (sp.count(0)) {
            for (int v : sp[0]) std::cout << v << " ";
        }
        std::cout << "\n\n";
    }

    // 2. Complete graph test (n = 5)
    {
        std::cout << "[Test 2] Complete Graph K5:\n";
        Graph g = generate_complete_graph(5);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);
        auto sp = shortest_paths(g, 0);

        std::cout << "  Vertices: " << g.get_num_vertices() << ", Edges: " << g.get_num_edges() << " (Expected: 10)\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 1)\n";
        std::cout << "  Cycle detected: ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << cycle[i] << (i + 1 < cycle.size() ? " -> " : "");
        }
        std::cout << "\n  Shortest path from 4 back to 0: ";
        if (sp.count(4)) {
            for (size_t i = 0; i < sp[4].size(); ++i) {
                std::cout << sp[4][i] << (i + 1 < sp[4].size() ? " -> " : "");
            }
        }
        std::cout << "\n\n";
    }

    // 3. Empty graph test (n = 5)
    {
        std::cout << "[Test 3] Empty Graph (n = 5):\n";
        Graph g = generate_empty_graph(5);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);
        auto sp = shortest_paths(g, 0);

        std::cout << "  Vertices: " << g.get_num_vertices() << ", Edges: " << g.get_num_edges() << "\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 5)\n";
        std::cout << "  Cycle detected: " << (cycle.empty() ? "None (Acyclic, as expected)" : "Unexpected cycle") << "\n";
        std::cout << "  Reachable vertices from 0: " << sp.size() << " (Expected: 1, only source itself)\n";
        std::cout << "\n";
    }

    // 4. Binary Heap test (n = 7)
    {
        std::cout << "[Test 4] Binary Heap (n = 7):\n";
        Graph g = generate_heap(7);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);
        auto sp = shortest_paths(g, 0);

        std::cout << "  Vertices: " << g.get_num_vertices() << ", Edges: " << g.get_num_edges() << " (Expected: 6)\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 1)\n";
        std::cout << "  Cycle detected: " << (cycle.empty() ? "None (Tree is Acyclic, as expected)" : "Unexpected cycle") << "\n";
        std::cout << "  Shortest path from leaf 6 back to 0: ";
        if (sp.count(6)) {
            for (size_t i = 0; i < sp[6].size(); ++i) {
                std::cout << sp[6][i] << (i + 1 < sp[6].size() ? " -> " : "");
            }
        }
        std::cout << "\n\n";
    }

    // 5. Truncated Heap test (m = 2, n = 9)
    {
        std::cout << "[Test 5] Truncated Heap (m = 2, n = 9):\n";
        Graph g = generate_truncated_heap(2, 9);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);

        // Expected edges: n - 1 - 2m = 9 - 1 - 4 = 4 edges
        // Expected components: m + 1 = 3 components
        std::cout << "  Vertices: " << g.get_num_vertices() << " (Expected: 7), Edges: " << g.get_num_edges() << " (Expected: 4)\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 3)\n";
        std::cout << "  Cycle detected: " << (cycle.empty() ? "None (Acyclic, as expected)" : "Unexpected cycle") << "\n\n";
    }

    // 6. Equivalence mod k test (n = 10, k = 3)
    {
        std::cout << "[Test 6] Equivalence Mod k (n = 10, k = 3):\n";
        Graph g = generate_equivalence_mod_k(10, 3);
        auto comps = connected_components(g);
        auto cycle = one_cycle(g);

        std::cout << "  Vertices: " << g.get_num_vertices() << ", Edges: " << g.get_num_edges() << "\n";
        std::cout << "  Connected Components Count: " << comps.size() << " (Expected: 3)\n";
        std::cout << "  Cycle detected: ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << cycle[i] << (i + 1 < cycle.size() ? " -> " : "");
        }
        std::cout << "\n\n";
    }
}

struct BenchmarkRecord {
    std::string graph_type;
    int n;
    long long edges;
    double gen_time_ms;
    double cc_time_ms;
    size_t cc_count;
    double cycle_time_ms;
    bool has_cycle;
    double sp_time_ms;
    size_t sp_reachable;
    double peak_mem_mb;
};

void run_scaling_benchmarks(std::ostream& out) {
    out << "\n========================================================================================\n";
    out << "                       SCALING EXPERIMENTS & BENCHMARK REPORT                          \n";
    out << "========================================================================================\n\n";

    out << std::left 
        << std::setw(20) << "Graph Type"
        << std::setw(10) << "Nodes (N)"
        << std::setw(14) << "Edges (E)"
        << std::setw(12) << "Build (ms)"
        << std::setw(12) << "CC (ms)"
        << std::setw(8)  << "Comps"
        << std::setw(12) << "Cycle (ms)"
        << std::setw(8)  << "Cyclic"
        << std::setw(12) << "SP (ms)"
        << std::setw(12) << "PeakMem(MB)"
        << "\n";
    out << std::string(120, '-') << "\n";

    auto benchmark_single = [&](const std::string& name, const Graph& g, int n, double build_ms) {
        Timer timer;

        // Connected Components
        timer.reset();
        auto comps = connected_components(g);
        double cc_time = timer.elapsed_ms();

        // One Cycle
        timer.reset();
        auto cycle = one_cycle(g);
        double cycle_time = timer.elapsed_ms();

        // Shortest Paths from source 0
        timer.reset();
        auto sp = shortest_paths(g, 0);
        double sp_time = timer.elapsed_ms();

        double peak_mem = get_peak_memory_mb();

        out << std::left
            << std::setw(20) << name
            << std::setw(10) << n
            << std::setw(14) << g.get_num_edges()
            << std::setw(12) << std::fixed << std::setprecision(3) << build_ms
            << std::setw(12) << std::fixed << std::setprecision(3) << cc_time
            << std::setw(8)  << comps.size()
            << std::setw(12) << std::fixed << std::setprecision(3) << cycle_time
            << std::setw(8)  << (!cycle.empty() ? "Yes" : "No")
            << std::setw(12) << std::fixed << std::setprecision(3) << sp_time
            << std::setw(12) << std::fixed << std::setprecision(2) << peak_mem
            << "\n";
    };

    Timer t;

    // 1. n-cycle sweeps (Linear path length O(n), total path storage O(n^2))
    std::vector<int> cycle_sizes = {100, 500, 1000, 2000, 5000};
    for (int n : cycle_sizes) {
        t.reset();
        Graph g = generate_n_cycle(n);
        double b_time = t.elapsed_ms();
        benchmark_single("n-Cycle", g, n, b_time);
    }
    out << std::string(120, '-') << "\n";

    // 2. Binary Heap sweeps (O(log n) path length, sparse tree edges O(n))
    std::vector<int> heap_sizes = {1000, 10000, 50000, 100000, 200000};
    for (int n : heap_sizes) {
        t.reset();
        Graph g = generate_heap(n);
        double b_time = t.elapsed_ms();
        benchmark_single("Binary Heap", g, n, b_time);
    }
    out << std::string(120, '-') << "\n";

    // 3. Truncated Heap sweeps (m = n / 4)
    std::vector<int> theap_sizes = {1000, 10000, 50000, 100000, 200000};
    for (int n : theap_sizes) {
        int m = n / 4;
        t.reset();
        Graph g = generate_truncated_heap(m, n);
        double b_time = t.elapsed_ms();
        benchmark_single("Truncated Heap", g, g.get_num_vertices(), b_time);
    }
    out << std::string(120, '-') << "\n";

    // 4. Empty Graph sweeps (O(n) components, 0 edges)
    std::vector<int> empty_sizes = {1000, 10000, 50000, 100000, 200000};
    for (int n : empty_sizes) {
        t.reset();
        Graph g = generate_empty_graph(n);
        double b_time = t.elapsed_ms();
        benchmark_single("Empty Graph", g, n, b_time);
    }
    out << std::string(120, '-') << "\n";

    // 5. Complete Graph sweeps (Dense O(n^2) edges, unit path length)
    std::vector<int> complete_sizes = {50, 100, 250, 500, 1000, 1500};
    for (int n : complete_sizes) {
        t.reset();
        Graph g = generate_complete_graph(n);
        double b_time = t.elapsed_ms();
        benchmark_single("Complete (Kn)", g, n, b_time);
    }
    out << std::string(120, '-') << "\n";

    // 6. Equivalence Mod k sweeps (k = 10, 10 dense cliques)
    std::vector<int> eq_sizes = {100, 250, 500, 1000, 2000};
    for (int n : eq_sizes) {
        t.reset();
        Graph g = generate_equivalence_mod_k(n, 10);
        double b_time = t.elapsed_ms();
        benchmark_single("Equiv Mod 10", g, n, b_time);
    }
    out << std::string(120, '-') << "\n";
}

int main() {
    std::cout << "Starting Graph Algorithm Simulation & Testing Suite...\n";

    run_correctness_tests();

    std::cout << "\nRunning full scaling benchmark matrix...\n";
    run_scaling_benchmarks(std::cout);

    // Also write report to docs/simulated_results.txt
    std::ofstream fout("docs/simulated_results.txt");
    if (fout.is_open()) {
        run_scaling_benchmarks(fout);
        fout.close();
        std::cout << "\nResults also written to docs/simulated_results.txt\n";
    }

    std::cout << "\nSimulated testing completed successfully.\n";
    return 0;
}
