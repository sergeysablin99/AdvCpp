//
// Created by Сергей on 25.03.2020.
//
#include "base_logger.h"

using namespace logger;

BaseLogger::BaseLogger(Level level) noexcept : level_(level)
{
}

void BaseLogger::debug(const std::string & msg) {
    log(msg, Level::DEBUG);
}

void BaseLogger::info(const std::string &msg) {
    log(msg, Level::INFO);
}

void BaseLogger::warn(const std::string & msg) {
    log(msg, Level::WARNING);
}

void BaseLogger::error(const std::string & msg) {
    log(msg, Level::ERROR);
}

void BaseLogger::set_level(Level level) noexcept {
    level_ = level;
}

Level BaseLogger::level() const noexcept {
    return level_;
}

std::string BaseLogger::get_level_prefix(Level level) {
    switch (level) {
        case Level::DEBUG :
            return "DEBUG: ";
        case Level::INFO :
            return "INFO: ";
        case Level::WARNING :
            return "WARNING: ";
        case Level::ERROR :
            return "ERROR: ";
        default:
            return "INFO: ";
    }
}

LoggerException::LoggerException(std::string message) : msg(std::move(message)){}
