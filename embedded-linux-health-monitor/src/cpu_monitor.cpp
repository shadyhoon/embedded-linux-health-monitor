#include "cpu_monitor.h"
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>

static bool readCpuTimes(unsigned long long& idle_all, unsigned long long& total) {
    std::ifstream stat("/proc/stat");
    if (!stat.is_open()) return false;
    std::string line;
    if (!std::getline(stat, line)) return false;
    unsigned long long user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0;
    int n = sscanf(line.c_str(), "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                   &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    if (n < 4) return false;
    idle_all = idle + iowait;
    total = user + nice + system + idle + iowait + irq + softirq + steal;
    return true;
}

CpuMonitor::CpuMonitor() : core_count_(0) {
    std::ifstream stat("/proc/stat");
    std::string line;
    while (std::getline(stat, line)) {
        if (line.size() > 3 && line.compare(0, 3, "cpu") == 0 &&
            line[3] >= '0' && line[3] <= '9') {
            ++core_count_;
        }
    }
}

CpuMonitor::~CpuMonitor() {}

double CpuMonitor::getUsage() const {
    unsigned long long idle1 = 0, total1 = 0, idle2 = 0, total2 = 0;
    if (!readCpuTimes(idle1, total1)) return 0.0;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!readCpuTimes(idle2, total2)) return 0.0;
    unsigned long long delta_total = total2 - total1;
    unsigned long long delta_idle = idle2 - idle1;
    if (delta_total == 0) return 0.0;
    double usage = static_cast<double>(delta_total - delta_idle) /
                   static_cast<double>(delta_total) * 100.0;
    if (usage < 0.0) usage = 0.0;
    if (usage > 100.0) usage = 100.0;
    return usage;
}

unsigned int CpuMonitor::getCoreCount() const {
    return core_count_;
}