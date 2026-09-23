#include "disk_monitor.h"
#include <fstream>

#ifdef __linux__
#include <sys/statvfs.h>
#else
#include <windows.h>
#endif

static bool readDiskSpace(long long& total, long long& available) {
#ifdef __linux__
    struct statvfs fs;
    if (statvfs("/", &fs) != 0) return false;
    total = static_cast<long long>(fs.f_blocks) * fs.f_frsize;
    available = static_cast<long long>(fs.f_bavail) * fs.f_frsize;
    return total > 0;
#else
    ULARGE_INTEGER free_to_caller, total_bytes, total_free;
    if (!GetDiskFreeSpaceExA("C:\\", &free_to_caller, &total_bytes, &total_free)) {
        return false;
    }
    total = static_cast<long long>(total_bytes.QuadPart);
    available = static_cast<long long>(free_to_caller.QuadPart);
    return total > 0;
#endif
}

DiskMonitor::DiskMonitor() : total_space_(0), available_space_(0) {}

DiskMonitor::~DiskMonitor() {}

double DiskMonitor::getUsage() const {
    long long total = 0, available = 0;
    if (!readDiskSpace(total, available)) {
        return 0.0;
    }
    return static_cast<double>(total - available) / static_cast<double>(total) * 100.0;
}

long long DiskMonitor::getTotalSpace() const {
    long long total = 0, available = 0;
    readDiskSpace(total, available);
    return total;
}

long long DiskMonitor::getAvailableSpace() const {
    long long total = 0, available = 0;
    readDiskSpace(total, available);
    return available;
}