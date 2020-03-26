//
// Created by Сергей on 26.03.2020.
//

#include "logger.h"

Logger& Logger::get_instance()
{
    static Logger logger;
    return logger;
}

Logger::Logger() {
    global_logger_ = std::make_unique<StdoutLogger>(BaseLogger::Level::INFO);
}

BaseLogger* Logger::get_global_logger() {
    return global_logger_.get();
}

void Logger::set_global_logger(BaseLogger *logger) {
    global_logger_.reset(logger);
}

FileLogger* create_file_logger(const std::string& path, BaseLogger::Level level) {
    Logger& logger = Logger::get_instance();
    logger.set_global_logger(new FileLogger(path, level));
    return dynamic_cast<FileLogger *>(logger.get_global_logger());
}


StdoutLogger* create_file_logger(BaseLogger::Level level) {
    Logger& logger = Logger::get_instance();
    logger.set_global_logger(new StdoutLogger(level));
    return dynamic_cast<StdoutLogger *>(logger.get_global_logger());
}


StderrLogger* create_stderr_logger(BaseLogger::Level level) {
    Logger& logger = Logger::get_instance();
    logger.set_global_logger(new StdoutLogger(level));
    return dynamic_cast<StderrLogger *>(logger.get_global_logger());
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