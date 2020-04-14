//
// Created by Сергей on 26.03.2020.
//

#include "logger.h"
#include "file_logger.h"
#include "stdout_logger.h"
#include "stderr_logger.h"


using namespace logger;

Logger& Logger::get_instance()
{
    static Logger logger;
    return logger;
}

Logger::Logger() : global_logger_{std::make_unique<StdoutLogger>(Level::INFO)}
{
}

BaseLogger* Logger::get_global_logger() {
    if (!global_logger_)
        throw LoggerException("Logger is empty");
    return global_logger_.get();
}

void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
    global_logger_ = std::move(logger);
}

namespace logger {
    std::unique_ptr<BaseLogger> create_file_logger(const std::string &path, Level level) {
        return std::make_unique<FileLogger>(path, level);
    }

    std::unique_ptr<BaseLogger> create_stdout_logger(Level level) {
        return std::make_unique<StdoutLogger>(level);
    }


    std::unique_ptr<BaseLogger> create_stderr_logger(Level level) {
        return std::make_unique<StderrLogger>(level);
    }

    void debug(const std::string &msg) {
        Logger::get_instance().get_global_logger()->debug(msg);
    }

    void info(const std::string &msg) {
        Logger::get_instance().get_global_logger()->info(msg);
    }

    void warning(const std::string &msg) {
        Logger::get_instance().get_global_logger()->warn(msg);
    }

    void error(const std::string &msg) {
        Logger::get_instance().get_global_logger()->error(msg);
    }
}