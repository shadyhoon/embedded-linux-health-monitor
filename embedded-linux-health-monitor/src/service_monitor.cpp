#include "service_monitor.h"
#include <cstdio>
#include <string>

#ifdef _WIN32
static const char* const QUIET_REDIRECT = " > NUL 2>&1";
#else
static const char* const QUIET_REDIRECT = " > /dev/null 2>&1";
#endif

static std::string shellQuote(const std::string& value) {
    return "'" + value + "'";
}

ServiceMonitor::ServiceMonitor() {}

ServiceMonitor::~ServiceMonitor() {}

bool ServiceMonitor::isRunning(const std::string& service_name) const {
    std::string command = "systemctl is-active --quiet " + shellQuote(service_name) + QUIET_REDIRECT;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return false;
    int rc = pclose(pipe);
    return rc == 0;
}

std::string ServiceMonitor::getStatus(const std::string& service_name) const {
    std::string command = "systemctl is-active " + shellQuote(service_name) + QUIET_REDIRECT;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "unknown";
    char buffer[128] = {0};
    std::string status;
    if (std::fgets(buffer, sizeof(buffer), pipe)) {
        status = buffer;
        while (!status.empty() && (status.back() == '\n' || status.back() == '\r' || status.back() == ' ')) {
            status.pop_back();
        }
    }
    pclose(pipe);
    if (status.empty()) return "unknown";
    return status;
}

bool ServiceMonitor::restartService(const std::string& service_name) const {
    std::string command = "systemctl restart " + shellQuote(service_name) + QUIET_REDIRECT;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return false;
    int rc = pclose(pipe);
    return rc == 0;
}