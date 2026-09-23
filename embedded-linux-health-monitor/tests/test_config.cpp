#include "config_manager.h"
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

int main(int argc, char** argv) {
    std::string config_path = (argc > 1) ? argv[1] : "config/health_monitor.json";

    ConfigManager manager;
    CHECK(manager.loadConfig(config_path));
    HealthConfig config = manager.getConfig();
    std::printf("test_config: cpu=%.1f ram=%.1f disk=%.1f temp=%.1f service=%s interval=%d auto=%d\n",
                config.cpu_threshold, config.ram_threshold, config.disk_threshold,
                config.temp_threshold, config.critical_service.c_str(),
                config.check_interval, config.auto_recovery ? 1 : 0);
    CHECK(config.cpu_threshold == 80.0);
    CHECK(config.ram_threshold == 80.0);
    CHECK(config.disk_threshold == 80.0);
    CHECK(config.temp_threshold == 80.0);
    CHECK(config.critical_service == "ssh");
    CHECK(config.check_interval == 60);
    CHECK(config.auto_recovery == true);

    ConfigManager missing;
    CHECK(!missing.loadConfig("this_file_does_not_exist.json"));
    HealthConfig defaults = missing.getConfig();
    CHECK(defaults.cpu_threshold == 80.0);
    CHECK(defaults.check_interval == 60);
    CHECK(defaults.critical_service == "ssh");

    std::printf("test_config PASSED\n");
    return 0;
}