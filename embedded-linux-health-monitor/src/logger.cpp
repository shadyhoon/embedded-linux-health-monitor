#include "logger.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>

Logger::Logger() : log_stream_(), log_filepath_("health_monitor.log") {
    log_stream_.open(log_filepath_, std::ios::app);
}

Logger::~Logger() {
    if (log_stream_.is_open()) {
        log_stream_.close();
    }
}

void Logger::log(const std::string& message) {
    if (!log_stream_.is_open()) {
        log_stream_.open(log_filepath_, std::ios::app);
    }
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::ostringstream line;
    line << "[" << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S") << "] " << message;
    log_stream_ << line.str() << std::endl;
    std::cout << line.str() << std::endl;
}

void Logger::setLogFile(const std::string& filepath) {
    log_filepath_ = filepath;
    if (log_stream_.is_open()) {
        log_stream_.close();
    }
    log_stream_.open(filepath, std::ios::app);
}