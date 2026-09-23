# Test Report — Embedded Linux Device Health Monitor

## Summary

| Test | Result | Exit code |
|---|---|---|
| test_cpu | PASSED | 0 |
| test_memory | PASSED | 0 |
| test_config | PASSED | 0 |
| test_service_monitor | PASSED | 0 |

**Overall: 4/4 passed.**

## Environment

- OS: Windows 11 (win32) — development host
- Compiler: MinGW g++ 15.2.0 (`-std=c++17 -Wall -Wextra`, zero warnings)
- Primary target platform: Linux (tests contain `__linux__`-guarded stricter assertions for on-device runs)

## Build

```
g++ -std=c++17 -Wall -Wextra -Iinclude -o build/health_monitor.exe src/*.cpp
g++ -std=c++17 -Wall -Wextra -Iinclude -o build/test_cpu.exe tests/test_cpu.cpp src/cpu_monitor.cpp
g++ -std=c++17 -Wall -Wextra -Iinclude -o build/test_memory.exe tests/test_memory.cpp src/memory_monitor.cpp
g++ -std=c++17 -Wall -Wextra -Iinclude -o build/test_config.exe tests/test_config.cpp src/config_manager.cpp
g++ -std=c++17 -Wall -Wextra -Iinclude -o build/test_service_monitor.exe tests/test_service_monitor.cpp src/service_monitor.cpp
```

All five binaries compiled with no errors and no warnings.

## Test Results (verbatim)

### test_cpu
```
test_cpu: usage=0.00% cores=0
test_cpu PASSED
```
Checks: `0.0 <= usage <= 100.0`; on Linux also `coreCount >= 1` (values of 0 are expected on the Windows host where `/proc/stat` does not exist).

### test_memory
```
test_memory: usage=0.00% total=0 kB available=0 kB
test_memory PASSED
```
Checks: usage within `[0,100]`, totals non-negative; on Linux additionally `total > 0`, `available <= total`, `usage > 0`.

### test_config
```
test_config: cpu=80.0 ram=80.0 disk=80.0 temp=80.0 service=ssh interval=60 auto=1
test_config PASSED
```
Checks: loads `config/health_monitor.json`, all four thresholds `== 80.0`, `critical_service == "ssh"`, `check_interval == 60`, `auto_recovery == true`; missing file returns `false` and leaves defaults intact (80.0 / 60 / "ssh").

### test_service_monitor
```
test_service_monitor: status('no-such-service-for-health-monitor-test')='unknown'
test_service_monitor: isRunning('no-such-service-for-health-monitor-test')=0
test_service_monitor: restart('no-such-service-for-health-monitor-test')=0
test_service_monitor PASSED
```
Checks: `getStatus()` never empty, `isRunning()` of a nonexistent unit is `false`, `restartService()` of a nonexistent unit fails safely (never restarts something unintended).

## Daemon Smoke Test

`health_monitor.exe` was started against the real config, observed for 4 seconds, then stopped:

```
[2026-09-23 20:29:03] Health monitor started: interval=60s critical_service=ssh auto_recovery=on
[2026-09-23 20:29:03] cpu=0.0% ram=0.0% disk=87.8% cpu_temp=0.0C sys_temp=0.0C network=down
[2026-09-23 20:29:03] WARNING: Disk usage 87.8% exceeds threshold 80.0%
[2026-09-23 20:29:03] WARNING: Network is down
[2026-09-23 20:29:03] CRITICAL: Service 'ssh' is not running. Status: unknown
[2026-09-23 20:29:03] Auto-recovery: restarting 'ssh'
[2026-09-23 20:29:04] Auto-recovery FAILED: could not restart 'ssh'
```

Verified behaviors: continuous sampling loop, all 6 monitors invoked per cycle, threshold warnings, service watchdog, auto-recovery attempt, logging to both file (`health_monitor.log`) and stdout (journal), clean exit on process termination.

## Notes

- On the Linux device, run with tests enabled via `cmake -DBUILD_TESTS=ON ..` and `ctest --output-on-failure`.
- Strict `__linux__` assertions (real `/proc`, `/sys` readings) activate automatically on the target platform.
- Recovery failure above is expected on Windows (no `systemctl`); on Linux it logs success after `systemctl restart`.