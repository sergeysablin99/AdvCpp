//
// Created by Сергей on 25.03.2020.
//
#include <base_logger.h>

BaseLogger::BaseLogger(BaseLogger::Level level) noexcept : level_(level)
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

void BaseLogger::set_level(enum BaseLogger::Level level) noexcept {
    level_ = level;
}

BaseLogger::Level BaseLogger::level() noexcept {
    return level_;
}

LoggerException::LoggerException(std::string message) : msg(std::move(message)){}
