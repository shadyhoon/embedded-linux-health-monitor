#include "memory_monitor.h"
#include <fstream>
#include <sstream>
#include <cstdio>

static bool readMeminfo(long long& total, long long& available) {
    std::ifstream stat("/proc/meminfo");
    if (!stat.is_open()) return false;
    std::string line;
    total = 0;
    available = 0;
    while (std::getline(stat, line)) {
        if (line.find("MemTotal:") == 0) {
            sscanf(line.c_str(), "MemTotal: %lld", &total);
        } else if (line.find("MemAvailable:") == 0) {
            sscanf(line.c_str(), "MemAvailable: %lld", &available);
        }
    }
    return total > 0;
}

MemoryMonitor::MemoryMonitor() : total_memory_(0), available_memory_(0) {}

MemoryMonitor::~MemoryMonitor() {}

double MemoryMonitor::getUsage() const {
    long long total = 0, available = 0;
    if (!readMeminfo(total, available)) {
        return 0.0;
    }
    return static_cast<double>(total - available) / static_cast<double>(total) * 100.0;
}

long long MemoryMonitor::getTotalMemory() const {
    long long total = 0, available = 0;
    readMeminfo(total, available);
    return total;
}

long long MemoryMonitor::getAvailableMemory() const {
    long long total = 0, available = 0;
    readMeminfo(total, available);
    return available;
}