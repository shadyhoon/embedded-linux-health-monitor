#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

class NetworkMonitor {
public:
    NetworkMonitor();
    ~NetworkMonitor();

    bool isConnected() const;
    double getDownloadSpeed() const;
    double getUploadSpeed() const;

private:
    mutable bool connected_;
};

#endif // NETWORK_MONITOR_H