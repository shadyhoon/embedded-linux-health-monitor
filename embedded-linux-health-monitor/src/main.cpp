#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "temperature_monitor.h"
#include "network_monitor.h"
#include "service_monitor.h"
#include "logger.h"
#include "config_manager.h"

#include <csignal>
#include <chrono>
#include <thread>
#include <sstream>
#include <string>
#include <cstdio>

static volatile std::sig_atomic_t g_running = 1;

static void handleSignal(int) {
    g_running = 0;
}

static std::string formatDouble(double value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.1f", value);
    return buffer;
}

int main(int argc, char** argv) {
    std::signal(SIGINT, handleSignal);
#ifdef SIGTERM
    std::signal(SIGTERM, handleSignal);
#endif

    ConfigManager config_manager;
    HealthConfig config;
    bool loaded = false;
    if (argc > 1) {
        loaded = config_manager.loadConfig(argv[1]);
    } else {
        loaded = config_manager.loadConfig("config/health_monitor.json");
        if (!loaded) {
            loaded = config_manager.loadConfig("/etc/health-monitor/health_monitor.json");
        }
    }
    if (loaded) {
        config = config_manager.getConfig();
    } else {
        HealthConfig defaults;
        defaults.cpu_threshold = 80.0;
        defaults.ram_threshold = 80.0;
        defaults.disk_threshold = 80.0;
        defaults.temp_threshold = 80.0;
        defaults.critical_service = "ssh";
        defaults.check_interval = 60;
        defaults.auto_recovery = true;
        config = defaults;
    }
    if (config.check_interval < 1) {
        config.check_interval = 1;
    }

    Logger logger;
    logger.log("Health monitor started: interval=" + std::to_string(config.check_interval) +
               "s critical_service=" + config.critical_service +
               " auto_recovery=" + (config.auto_recovery ? "on" : "off"));

    CpuMonitor cpu_monitor;
    MemoryMonitor memory_monitor;
    DiskMonitor disk_monitor;
    TemperatureMonitor temperature_monitor;
    NetworkMonitor network_monitor;
    ServiceMonitor service_monitor;

    while (g_running) {
        double cpu_usage = cpu_monitor.getUsage();
        double ram_usage = memory_monitor.getUsage();
        double disk_usage = disk_monitor.getUsage();
        double cpu_temp = temperature_monitor.getCpuTemperature();
        double sys_temp = temperature_monitor.getSystemTemperature();
        bool network_ok = network_monitor.isConnected();

        std::ostringstream summary;
        summary << "cpu=" << formatDouble(cpu_usage) << "% "
                << "ram=" << formatDouble(ram_usage) << "% "
                << "disk=" << formatDouble(disk_usage) << "% "
                << "cpu_temp=" << formatDouble(cpu_temp) << "C "
                << "sys_temp=" << formatDouble(sys_temp) << "C "
                << "network=" << (network_ok ? "up" : "down");
        logger.log(summary.str());

        if (cpu_usage > config.cpu_threshold) {
            logger.log("WARNING: CPU usage " + formatDouble(cpu_usage) +
                       "% exceeds threshold " + formatDouble(config.cpu_threshold) + "%");
        }
        if (ram_usage > config.ram_threshold) {
            logger.log("WARNING: RAM usage " + formatDouble(ram_usage) +
                       "% exceeds threshold " + formatDouble(config.ram_threshold) + "%");
        }
        if (disk_usage > config.disk_threshold) {
            logger.log("WARNING: Disk usage " + formatDouble(disk_usage) +
                       "% exceeds threshold " + formatDouble(config.disk_threshold) + "%");
        }
        if (cpu_temp > config.temp_threshold) {
            logger.log("WARNING: CPU temperature " + formatDouble(cpu_temp) +
                       "C exceeds threshold " + formatDouble(config.temp_threshold) + "C");
        }
        if (!network_ok) {
            logger.log("WARNING: Network is down");
        }

        if (!service_monitor.isRunning(config.critical_service)) {
            logger.log("CRITICAL: Service '" + config.critical_service + "' is not running. Status: " +
                       service_monitor.getStatus(config.critical_service));
            if (config.auto_recovery) {
                logger.log("Auto-recovery: restarting '" + config.critical_service + "'");
                if (service_monitor.restartService(config.critical_service)) {
                    logger.log("Auto-recovery: '" + config.critical_service + "' restarted successfully");
                } else {
                    logger.log("Auto-recovery FAILED: could not restart '" +
                               config.critical_service + "'");
                }
            }
        }

        for (int i = 0; i < config.check_interval && g_running; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    logger.log("Health monitor stopped (signal received)");
    return 0;
}