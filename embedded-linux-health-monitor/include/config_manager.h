#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>

struct HealthConfig {
    double cpu_threshold;
    double ram_threshold;
    double disk_threshold;
    double temp_threshold;
    std::string critical_service;
    int check_interval;
    bool auto_recovery;
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    bool loadConfig(const std::string& filepath);
    HealthConfig getConfig() const;

private:
    HealthConfig config_;
};

#endif // CONFIG_MANAGER_H