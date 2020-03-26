//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_BASE_LOGGER_H
#define LOG_BASE_LOGGER_H

#include <string>
#include <utility>

class BaseLogger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    explicit BaseLogger(Level level = Level::INFO) noexcept;

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
    void set_level(Level level) noexcept;
    Level level() noexcept;
    virtual void flush() = 0;

    ~BaseLogger() = default;
private:
    Level level_;
    virtual void log(const std::string& msg, Level) = 0;
};

class LoggerException : public std::exception {
private:
    std::string msg;
public:
    explicit LoggerException(std::string message);
    const char * what() noexcept { return msg.c_str(); };
};

#endif //LOG_BASE_LOGGER_H
