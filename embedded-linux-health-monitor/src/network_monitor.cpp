#include "network_monitor.h"
#include <fstream>
#include <sstream>

NetworkMonitor::NetworkMonitor() : connected_(false) {}

NetworkMonitor::~NetworkMonitor() {}

bool NetworkMonitor::isConnected() const {
#ifdef __linux__
    std::ifstream stat("/proc/net/dev");
    std::string line;
    while (std::getline(stat, line)) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos || start >= colon) continue;
        std::string iface = line.substr(start, colon - start);
        if (iface == "lo" || iface.empty()) continue;

        std::ifstream operstate("/sys/class/net/" + iface + "/operstate");
        std::string state;
        if (operstate >> state && state == "up") {
            connected_ = true;
            return true;
        }
        std::ifstream carrier("/sys/class/net/" + iface + "/carrier");
        int flag = 0;
        if (carrier >> flag && flag == 1) {
            connected_ = true;
            return true;
        }
    }
    connected_ = false;
    return false;
#else
    connected_ = false;
    return false;
#endif
}

double NetworkMonitor::getDownloadSpeed() const {
    return 0.0;
}

double NetworkMonitor::getUploadSpeed() const {
    return 0.0;
}