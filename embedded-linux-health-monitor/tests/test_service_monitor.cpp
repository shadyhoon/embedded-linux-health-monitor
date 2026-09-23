#include "service_monitor.h"
#include <cstdio>
#include <string>

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            std::fprintf(stderr, "FAILED: %s (%s:%d)\n", #cond, __FILE__, \
                         __LINE__);                                        \
            return 1;                                                      \
        }                                                                  \
    } while (0)

int main() {
    ServiceMonitor monitor;
    const std::string fake = "no-such-service-for-health-monitor-test";

    std::string status = monitor.getStatus(fake);
    std::printf("test_service_monitor: status('%s')='%s'\n", fake.c_str(), status.c_str());
    CHECK(!status.empty());

    bool running = monitor.isRunning(fake);
    std::printf("test_service_monitor: isRunning('%s')=%d\n", fake.c_str(), running ? 1 : 0);
    CHECK(running == false);

    bool restarted = monitor.restartService(fake);
    std::printf("test_service_monitor: restart('%s')=%d\n", fake.c_str(), restarted ? 1 : 0);
    CHECK(restarted == false);

    std::printf("test_service_monitor PASSED\n");
    return 0;
}