#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

class DiskMonitor {
public:
    DiskMonitor();
    ~DiskMonitor();

    double getUsage() const;
    long long getTotalSpace() const;
    long long getAvailableSpace() const;

private:
    long long total_space_;
    long long available_space_;
};

#endif // DISK_MONITOR_H