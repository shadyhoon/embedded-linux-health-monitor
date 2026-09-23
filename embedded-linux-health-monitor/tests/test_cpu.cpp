#include "cpu_monitor.h"
#include <cstdio>

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            std::fprintf(stderr, "FAILED: %s (%s:%d)\n", #cond, __FILE__, \
                         __LINE__);                                        \
            return 1;                                                      \
        }                                                                  \
    } while (0)

int main() {
    CpuMonitor cpu;
    double usage = cpu.getUsage();
    std::printf("test_cpu: usage=%.2f%% cores=%u\n", usage, cpu.getCoreCount());
    CHECK(usage >= 0.0 && usage <= 100.0);
#ifdef __linux__
    CHECK(cpu.getCoreCount() >= 1);
#endif
    std::printf("test_cpu PASSED\n");
    return 0;
}