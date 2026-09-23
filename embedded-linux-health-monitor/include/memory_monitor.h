#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

class MemoryMonitor {
public:
    MemoryMonitor();
    ~MemoryMonitor();

    double getUsage() const;
    long long getTotalMemory() const;
    long long getAvailableMemory() const;

private:
    long long total_memory_;
    long long available_memory_;
};

#endif // MEMORY_MONITOR_H