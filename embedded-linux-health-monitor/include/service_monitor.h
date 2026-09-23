#ifndef SERVICE_MONITOR_H
#define SERVICE_MONITOR_H

#include <string>

class ServiceMonitor {
public:
    ServiceMonitor();
    ~ServiceMonitor();

    bool isRunning(const std::string& service_name) const;
    std::string getStatus(const std::string& service_name) const;
    bool restartService(const std::string& service_name) const;

private:
    // Track critical service state
};

#endif // SERVICE_MONITOR_H