//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stdout_logger.h"

using namespace logger;

StdoutLogger::StdoutLogger(Level level) noexcept : BaseLogger(level) {}

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

void StdoutLogger::log(const std::string &msg, Level lvl) {
    if (lvl >= level())
        std::cout << get_level_prefix(lvl) << msg << std::endl;
}
