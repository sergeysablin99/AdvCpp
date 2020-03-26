//
// Created by Сергей on 26.03.2020.
//

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H

#include <file_logger.h>
#include <memory>
#include <stdout_logger.h>
#include <stderr_logger.h>


class Logger
{
private:
    std::unique_ptr<BaseLogger> global_logger_;
    explicit Logger();

public:
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;

    static Logger& get_instance();
    BaseLogger* get_global_logger();
    void set_global_logger(BaseLogger* logger);
};

FileLogger* create_file_logger(const std::string& path, BaseLogger::Level level);
StdoutLogger* create_file_logger(BaseLogger::Level level);
StderrLogger* create_stderr_logger(BaseLogger::Level level);

void debug(const std::string& msg);
void info(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);

#endif //LOG_LOGGER_H
