#include "memory_monitor.h"
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
    MemoryMonitor memory;
    double usage = memory.getUsage();
    long long total = memory.getTotalMemory();
    long long available = memory.getAvailableMemory();
    std::printf("test_memory: usage=%.2f%% total=%lld kB available=%lld kB\n",
                usage, total, available);
    CHECK(usage >= 0.0 && usage <= 100.0);
    CHECK(total >= 0);
    CHECK(available >= 0);
#ifdef __linux__
    CHECK(total > 0);
    CHECK(available <= total);
    CHECK(usage > 0.0);
#endif
    std::printf("test_memory PASSED\n");
    return 0;
}