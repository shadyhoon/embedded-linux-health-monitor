#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
public:
    Logger();
    ~Logger();

    void log(const std::string& message);
    void setLogFile(const std::string& filepath);

private:
    std::ofstream log_stream_;
    std::string log_filepath_;
};

#endif // LOGGER_H