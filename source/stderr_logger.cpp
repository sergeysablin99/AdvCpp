//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stderr_logger.h"

using namespace logger;

StderrLogger::StderrLogger(Level level) noexcept : BaseLogger(level) {}

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

void StderrLogger::log(const std::string &msg, Level lvl) {
    if (lvl >= level())
        std::cerr << get_level_prefix(lvl) << msg << std::endl;
}
