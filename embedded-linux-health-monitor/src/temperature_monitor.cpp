#include "temperature_monitor.h"
#include <fstream>

static double readThermalZone(int zone) {
    std::ifstream stat("/sys/class/thermal/thermal_zone" + std::to_string(zone) + "/temp");
    long temp = 0;
    if (stat >> temp) {
        return static_cast<double>(temp) / 1000.0;
    }
    return 0.0;
}

TemperatureMonitor::TemperatureMonitor() : cpu_temp_(0.0), system_temp_(0.0) {}

TemperatureMonitor::~TemperatureMonitor() {}

double TemperatureMonitor::getCpuTemperature() const {
    return readThermalZone(0);
}

double TemperatureMonitor::getSystemTemperature() const {
    double zone1 = readThermalZone(1);
    if (zone1 > 0.0) return zone1;
    return readThermalZone(0);
}