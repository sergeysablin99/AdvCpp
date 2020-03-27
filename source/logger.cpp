//
// Created by Сергей on 26.03.2020.
//

#include "logger.h"

using namespace logger;
Logger& Logger::get_instance()
{
    static Logger logger;
    return logger;
}

Logger::Logger() {
    global_logger_ = std::make_unique<StdoutLogger>(Level::INFO);
}

std::unique_ptr<BaseLogger> Logger::get_global_logger() {
    return std::move(global_logger_);
}

void Logger::set_global_logger(BaseLogger&& logger) {
    global_logger_.reset(&logger);
}

std::unique_ptr<BaseLogger> create_file_logger(const std::string& path, Level level) {
    Logger::get_instance().set_global_logger(std::move(FileLogger(path, level)));
    return Logger::get_instance().get_global_logger();
}


std::unique_ptr<BaseLogger> create_stdout_logger(Level level) {
    Logger::get_instance().set_global_logger(std::move(StdoutLogger(level)));
    return Logger::get_instance().get_global_logger();
}


std::unique_ptr<BaseLogger> create_stderr_logger(Level level) {
    Logger::get_instance().set_global_logger(std::move(StdoutLogger(level)));
    return Logger::get_instance().get_global_logger();
}


void debug(const std::string& msg) {
    Logger::get_instance().get_global_logger()->debug(msg);
}

void info(const std::string& msg) {
    Logger::get_instance().get_global_logger()->info(msg);
}

void warning(const std::string& msg) {
    Logger::get_instance().get_global_logger()->warn(msg);
}

void error(const std::string& msg) {
    Logger::get_instance().get_global_logger()->error(msg);
}