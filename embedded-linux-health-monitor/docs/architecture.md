# Architecture — Embedded Linux Device Health Monitor & Auto-Recovery Agent

## Purpose

A C++17 daemon that runs continuously in the background on a Linux device,
samples six health signals, compares them against configured thresholds, logs
findings, and automatically restarts one critical systemd service when it
becomes unhealthy.

## Components

| Component | Header | Source | Responsibility |
|---|---|---|---|
| CpuMonitor | `include/cpu_monitor.h` | `src/cpu_monitor.cpp` | CPU usage % from `/proc/stat` (delta between two samples), core count |
| MemoryMonitor | `include/memory_monitor.h` | `src/memory_monitor.cpp` | RAM usage % from `/proc/meminfo` (MemTotal / MemAvailable) |
| DiskMonitor | `include/disk_monitor.h` | `src/disk_monitor.cpp` | Disk usage % via `statvfs("/")` |
| TemperatureMonitor | `include/temperature_monitor.h` | `src/temperature_monitor.cpp` | CPU/system temperature from `/sys/class/thermal/thermal_zone*/temp` |
| NetworkMonitor | `include/network_monitor.h` | `src/network_monitor.cpp` | Link status from `/proc/net/dev` + `/sys/class/net/<iface>/operstate` |
| ServiceMonitor | `include/service_monitor.h` | `src/service_monitor.cpp` | Status/restart of the critical systemd service via `systemctl` |
| Logger | `include/logger.h` | `src/logger.cpp` | Timestamped logging to file and stdout (journal) |
| ConfigManager | `include/config_manager.h` | `src/config_manager.cpp` | Loads flat JSON config with thresholds, interval, service name |
| Main loop | — | `src/main.cpp` | Signal handling, sampling loop, threshold checks, auto-recovery |

## Control flow

```
SIGINT/SIGTERM ──► g_running = 0
                         ▲
                         │
   ┌─────────────────────┴──────────────────────┐
   │  while (g_running)                        │
   │    1. sample CPU, RAM, disk, temp, network │
   │    2. log one status summary line          │
   │    3. compare each metric to threshold ────┼──► WARNING log
   │    4. query critical service state         │
   │       └─ not running ──► auto_recovery?    │
   │            └─ yes ──► systemctl restart ───┼──► recovery log
   │    5. sleep check_interval (1 s steps)     │
   └────────────────────────────────────────────┘
```

## Configuration

`config/health_monitor.json` (installed to `/etc/health-monitor/`):

- `cpu_threshold`, `ram_threshold`, `disk_threshold`, `temp_threshold` — percent/°C limits
- `critical_service` — systemd unit name to watchdog
- `check_interval` — seconds between samples
- `auto_recovery` — enables the restart action

Lookup order in `main`: argv path → `config/health_monitor.json` →
`/etc/health-monitor/health_monitor.json` → built-in defaults.

## Lifecycle

- Packaged as a systemd unit (`systemd/device-health-monitor.service`) with
  `Restart=on-failure` so the supervisor itself is also supervised.
- `scripts/install.sh` installs binary, config, and unit; `scripts/uninstall.sh`
  removes them; `scripts/simulate_failure.sh` safely stops the watched service
  (stop only, nothing disabled or deleted) to demonstrate detection + recovery.

## Portability note

Linux paths (`/proc`, `/sys`, `statvfs`) are the primary target. The disk and
network collectors contain small `#ifdef` fallbacks so the project also builds
and its unit tests run on a Windows/MinGW host used for development.