//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stdout_logger.h"

StdoutLogger::StdoutLogger(StdoutLogger::Level level) noexcept : BaseLogger(level) {}

void StdoutLogger::flush() {
    try {
        std::cout.flush();
    }
    catch (const std::ios_base::failure& e) {
        throw LoggerException(e.what());
    }
    if (!std::cout.good()) {
        std::cout.clear();
        throw LoggerException("Bad flush");
    }
}

void StdoutLogger::log(const std::string &msg, BaseLogger::Level level) {
    if (level >= this->level()) {
        switch (level) {
            case Level::DEBUG :
                std::cout << "DEBUG: ";
                break;
            case Level ::INFO :
                std::cout << "INFO: ";
                break;
            case Level::WARNING :
                std::cout << "WARNING: ";
                break;
            case Level::ERROR :
                std::cout << "ERROR: ";
                break;
        }
        std::cout << msg << std::endl;
    }
}
