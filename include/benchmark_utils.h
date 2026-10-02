#ifndef BENCHMARK_UTILS_H
#define BENCHMARK_UTILS_H

#include <chrono>
#include <cstddef>

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

/**
 * @brief High-resolution stopwatch timer for measuring elapsed CPU/wall-clock execution time.
 */
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_point;

public:
    Timer() {
        reset();
    }

    void reset() {
        start_point = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms() const {
        auto end_point = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_point - start_point;
        return duration.count();
    }

    double elapsed_sec() const {
        return elapsed_ms() / 1000.0;
    }
};

/**
 * @brief Queries the OS kernel for the peak physical memory (Working Set) consumed by the process.
 * 
 * @return size_t Peak memory in bytes.
 */
inline size_t get_peak_memory_bytes() {
#if defined(_WIN32) || defined(_WIN64)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PeakWorkingSetSize;
    }
    return 0;
#else
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // On Linux ru_maxrss is in kilobytes; on macOS it is in bytes
#if defined(__APPLE__)
        return static_cast<size_t>(usage.ru_maxrss);
#else
        return static_cast<size_t>(usage.ru_maxrss) * 1024;
#endif
    }
    return 0;
#endif
}

/**
 * @brief Returns the peak physical memory in Megabytes (MB).
 */
inline double get_peak_memory_mb() {
    return static_cast<double>(get_peak_memory_bytes()) / (1024.0 * 1024.0);
}

#endif // BENCHMARK_UTILS_H
