#include "config_manager.h"
#include <fstream>
#include <sstream>
#include <cctype>

static bool extractJsonValue(const std::string& text, const std::string& key, std::string& out) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = text.find(pattern);
    if (pos == std::string::npos) return false;
    pos = text.find(':', pos + pattern.size());
    if (pos == std::string::npos) return false;
    ++pos;
    while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) ++pos;
    if (pos >= text.size()) return false;
    if (text[pos] == '"') {
        size_t end = text.find('"', pos + 1);
        if (end == std::string::npos) return false;
        out = text.substr(pos + 1, end - pos - 1);
        return true;
    }
    size_t end = pos;
    while (end < text.size() && text[end] != ',' && text[end] != '}' &&
           text[end] != '\n' && text[end] != '\r') {
        ++end;
    }
    out = text.substr(pos, end - pos);
    while (!out.empty() && std::isspace(static_cast<unsigned char>(out.back()))) out.pop_back();
    return !out.empty();
}

ConfigManager::ConfigManager() : config_{} {
    config_.cpu_threshold = 80.0;
    config_.ram_threshold = 80.0;
    config_.disk_threshold = 80.0;
    config_.temp_threshold = 80.0;
    config_.critical_service = "ssh";
    config_.check_interval = 60;
    config_.auto_recovery = true;
}

ConfigManager::~ConfigManager() {}

bool ConfigManager::loadConfig(const std::string& filepath) {
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        return false;
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string text = buffer.str();

    std::string value;
    if (extractJsonValue(text, "cpu_threshold", value)) {
        config_.cpu_threshold = std::stod(value);
    }
    if (extractJsonValue(text, "ram_threshold", value)) {
        config_.ram_threshold = std::stod(value);
    }
    if (extractJsonValue(text, "disk_threshold", value)) {
        config_.disk_threshold = std::stod(value);
    }
    if (extractJsonValue(text, "temp_threshold", value)) {
        config_.temp_threshold = std::stod(value);
    }
    if (extractJsonValue(text, "critical_service", value)) {
        config_.critical_service = value;
    }
    if (extractJsonValue(text, "check_interval", value)) {
        config_.check_interval = std::stoi(value);
    }
    if (extractJsonValue(text, "auto_recovery", value)) {
        config_.auto_recovery = (value == "true");
    }
    return true;
}

HealthConfig ConfigManager::getConfig() const {
    return config_;
}