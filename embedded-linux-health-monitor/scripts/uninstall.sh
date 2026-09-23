#!/bin/bash
set -e

echo "Uninstalling Embedded Linux Device Health Monitor..."

# Stop and disable service
systemctl stop device-health-monitor || true
systemctl disable device-health-monitor || true

# Remove systemd service
rm -f /etc/systemd/system/device-health-monitor.service
systemctl daemon-reload

# Remove binary
rm -f /usr/local/bin/health_monitor

# Remove configuration
rm -rf /etc/health-monitor

echo "Uninstallation complete."