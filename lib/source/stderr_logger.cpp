//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stderr_logger.h"

using namespace logger;

StderrLogger::StderrLogger(Level level) noexcept : BaseLogger(level) {}

void StderrLogger::flush() {
    TryFlush(std::cerr);
}

void StderrLogger::log(const std::string &msg, Level lvl) {
    if (lvl >= level())
        std::cerr << get_level_prefix(lvl) << msg << std::endl;
}
