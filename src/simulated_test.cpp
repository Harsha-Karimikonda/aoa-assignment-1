#include "graph_operations.h"
#include "graph_simulator.h"
#include "benchmark.h"
#include <iostream>
#include <iomanip>
#include <vector>

void test_correctness() {
    std::cout << "--- Correctness Tests ---\n";
    // 1. 6-Cycle
    Graph c6 = generate_n_cycle(6);
    auto c6_comp = connected_components(c6);
    auto c6_cyc = one_cycle(c6);
    auto c6_sp = shortest_paths(c6, 0);
    std::cout << "6-Cycle: Components=" << c6_comp.size() << " (Exp: 1), Cycle Len=" << c6_cyc.size() 
              << " (Exp: 7), SP[3] to 0 hops=" << (c6_sp.count(3) ? c6_sp[3].size() - 1 : 0) << " (Exp: 3)\n";

    // 2. Complete Graph K5
    Graph k5 = generate_complete_graph(5);
    std::cout << "K5: Components=" << connected_components(k5).size() << " (Exp: 1), Cycle Len=" 
              << one_cycle(k5).size() << " (Exp: >=4), SP[4] hops=" << shortest_paths(k5, 0)[4].size() - 1 << " (Exp: 1)\n";

    // 3. Binary Heap n=7
    Graph h7 = generate_heap(7);
    std::cout << "Heap(7): Components=" << connected_components(h7).size() << " (Exp: 1), Cycles=" 
              << (one_cycle(h7).empty() ? "None (Exp: None)" : "Found") << "\n";

    // 4. Truncated Heap m=2, n=9
    Graph th = generate_truncated_heap(2, 9);
    std::cout << "Truncated Heap(2,9): Components=" << connected_components(th).size() << " (Exp: 3), Cycles=" 
              << (one_cycle(th).empty() ? "None (Exp: None)" : "Found") << "\n";

    // 5. Equivalence mod 3 n=10
    Graph eq = generate_equivalence_mod_k(10, 3);
    std::cout << "Equiv mod 3(10): Components=" << connected_components(eq).size() << " (Exp: 3)\n\n";
}

#if !defined(_WIN32)
#include <unistd.h>
#include <sys/wait.h>
#endif

void run_single_bench(const std::string& name, const Graph& g, int n) {
    using clock = std::chrono::high_resolution_clock;

    auto t0 = clock::now();
    auto comps = connected_components(g);
    double t_cc = std::chrono::duration<double, std::milli>(clock::now() - t0).count();

    t0 = clock::now();
    auto cycle = one_cycle(g);
    double t_cyc = std::chrono::duration<double, std::milli>(clock::now() - t0).count();

    t0 = clock::now();
    auto sp = shortest_paths(g, 0);
    double t_sp = std::chrono::duration<double, std::milli>(clock::now() - t0).count();

    std::cout << std::left << std::setw(18) << name
              << std::setw(8) << n
              << std::setw(10) << std::fixed << std::setprecision(3) << t_cc
              << std::setw(8)  << comps.size()
              << std::setw(12) << t_cyc
              << std::setw(8)  << (!cycle.empty() ? "Yes" : "No")
              << std::setw(10) << t_sp
              << std::setw(10) << std::setprecision(2) << get_peak_memory_mb() << "\n" << std::flush;
}

template <typename Func>
void bench_graph(const std::string& name, int n, Func generator) {
#if !defined(_WIN32)
    pid_t pid = fork();
    if (pid == 0) {
        Graph g = generator();
        run_single_bench(name, g, n);
        _exit(0);
    } else if (pid > 0) {
        waitpid(pid, nullptr, 0);
    } else {
        Graph g = generator();
        run_single_bench(name, g, n);
    }
#else
    Graph g = generator();
    run_single_bench(name, g, n);
#endif
}

int main() {
    test_correctness();

    std::cout << "--- Scaling Benchmarks ---\n";
    std::cout << std::left << std::setw(18) << "Graph Type" << std::setw(8) << "N"
              << std::setw(10) << "CC (ms)" << std::setw(8) << "Comps"
              << std::setw(12) << "Cycle (ms)" << std::setw(8) << "Cyclic"
              << std::setw(10) << "SP (ms)" << std::setw(10) << "PeakMem(MB)\n";
    std::cout << std::string(76, '-') << "\n";

    // 1. n-cycle
    for (int n : {100, 500, 1000, 2000, 5000}) bench_graph("n-Cycle", n, [n](){ return generate_n_cycle(n); });

    // 2. Complete Graph Kn
    for (int n : {50, 100, 250, 500, 1000}) bench_graph("Complete Kn", n, [n](){ return generate_complete_graph(n); });

    // 3. Binary Heap
    for (int n : {1000, 10000, 50000, 100000}) bench_graph("Binary Heap", n, [n](){ return generate_heap(n); });

    // 4. Truncated Heap (m = n / 4)
    for (int n : {1000, 10000, 50000, 100000}) bench_graph("Trunc Heap", n - (n / 4), [n](){ return generate_truncated_heap(n / 4, n); });

    // 5. Empty Graph
    for (int n : {1000, 10000, 50000, 100000}) bench_graph("Empty Graph", n, [n](){ return generate_empty_graph(n); });

    // 6. Equivalence Mod 10
    for (int n : {100, 250, 500, 1000, 2000}) bench_graph("Equiv Mod 10", n, [n](){ return generate_equivalence_mod_k(n, 10); });

    return 0;
}
