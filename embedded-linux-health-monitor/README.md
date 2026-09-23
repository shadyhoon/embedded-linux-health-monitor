# Embedded Linux Device Health Monitor & Auto-Recovery Agent

## Overview

A C++17 health supervision system for Linux devices that monitors system health and enables automatic recovery.

## Monitored Metrics

1. **CPU Usage** - Percentage of CPU utilization across all cores
2. **RAM Usage** - Available vs total memory percentage
3. **Disk Usage** - Disk space utilization percentage
4. **CPU/System Temperature** - Thermal zone readings
5. **Network Status** - Interface connectivity status
6. **Critical Service Monitoring** - Watchdog for essential services

## Project Structure

```
embedded-linux-health-monitor/
├── config/           # Configuration files
├── include/          # Header files
├── src/              # Source files
├── systemd/          # systemd service unit
├── scripts/          # Installation and utility scripts
├── tests/            # Unit tests
└── docs/             # Documentation
```

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Configuration

Edit `config/health_monitor.json` to customize thresholds and behavior.

## Licensing

MIT License