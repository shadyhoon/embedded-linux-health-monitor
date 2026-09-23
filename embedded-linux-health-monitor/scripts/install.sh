#!/bin/bash
set -e

PACKAGE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${PACKAGE_DIR}/build/health_monitor"

echo "Installing Embedded Linux Device Health Monitor..."

# Create install directories
mkdir -p /usr/local/bin
mkdir -p /etc/health-monitor

# Copy binary
cp "${BUILD_DIR}" /usr/local/bin/health_monitor
chmod +x /usr/local/bin/health_monitor

# Copy configuration
cp "${PACKAGE_DIR}/config/health_monitor.json" /etc/health-monitor/

# Install systemd service
cp "${PACKAGE_DIR}/systemd/device-health-monitor.service" /etc/systemd/system/
systemctl daemon-reload
systemctl enable device-health-monitor

echo "Installation complete."
echo "Service enabled and starting..."
systemctl start device-health-monitor