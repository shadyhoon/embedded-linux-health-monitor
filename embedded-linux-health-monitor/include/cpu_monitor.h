#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

class CpuMonitor {
public:
    CpuMonitor();
    ~CpuMonitor();

    double getUsage() const;
    unsigned int getCoreCount() const;

private:
    unsigned int core_count_;
};

#endif // CPU_MONITOR_H