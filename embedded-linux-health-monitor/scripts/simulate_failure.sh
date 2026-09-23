#!/bin/bash
set -e

# Safe failure simulation: stops the critical service that the health monitor
# watches, so the monitor can detect the failure and restart it automatically.
# Nothing is disabled, masked, or deleted - a simple "systemctl stop" only.

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CONFIG_FILE="${SCRIPT_DIR}/../config/health_monitor.json"

if [ "$(id -u)" -ne 0 ]; then
    echo "Error: this script must be run as root (sudo)." >&2
    exit 1
fi

if [ ! -f "${CONFIG_FILE}" ]; then
    echo "Error: config not found: ${CONFIG_FILE}" >&2
    exit 1
fi

SERVICE="$(sed -n 's/.*"critical_service"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/p' "${CONFIG_FILE}" | head -n1)"

if [ -z "${SERVICE}" ]; then
    echo "Error: critical_service not set in ${CONFIG_FILE}" >&2
    exit 1
fi

if ! systemctl list-unit-files "${SERVICE}.service" >/dev/null 2>&1; then
    echo "Error: unknown systemd unit: ${SERVICE}.service" >&2
    exit 1
fi

echo "Simulating failure: stopping service '${SERVICE}' (stop only)..."
systemctl stop "${SERVICE}"

echo "Done. '${SERVICE}' is now stopped."
echo "The health monitor should log the failure and restart '${SERVICE}' automatically."
echo "Check with: systemctl status ${SERVICE} ; tail -f health_monitor.log"