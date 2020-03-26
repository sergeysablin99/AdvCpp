//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stderr_logger.h"

StderrLogger::StderrLogger(BaseLogger::Level level) noexcept : BaseLogger(level) {}

void StderrLogger::flush() {
    try {
        std::cerr.flush();
    }
    catch (const std::ios_base::failure& e) {
        throw LoggerException(e.what());
    }
    if (!std::cerr.good()) {
        std::cerr.clear();
        throw LoggerException("Bad flush");
    }
}

void StderrLogger::log(const std::string &msg, BaseLogger::Level level) {
    if (level >= this->level()) {
        switch (level) {
            case Level::DEBUG :
                std::cerr << "DEBUG: ";
                break;
            case Level ::INFO :
                std::cerr << "INFO: ";
                break;
            case Level::WARNING :
                std::cerr << "WARNING: ";
                break;
            case Level::ERROR :
                std::cerr << "ERROR: ";
                break;
        }
        std::cerr << msg << std::endl;
    }
}
