#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

inline double get_peak_memory_mb() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PeakWorkingSetSize / (1024.0 * 1024.0);
    }
#else
    struct rusage u;
    if (getrusage(RUSAGE_SELF, &u) == 0) return u.ru_maxrss / 1024.0;
#endif
    return 0.0;
}

#endif // BENCHMARK_H
