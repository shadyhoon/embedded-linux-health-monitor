#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

class TemperatureMonitor {
public:
    TemperatureMonitor();
    ~TemperatureMonitor();

    double getCpuTemperature() const;
    double getSystemTemperature() const;

private:
    double cpu_temp_;
    double system_temp_;
};

#endif // TEMPERATURE_MONITOR_H