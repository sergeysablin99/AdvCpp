//
// Created by Сергей on 25.03.2020.
//

#include <iostream>
#include "stdout_logger.h"

using namespace logger;

StdoutLogger::StdoutLogger(Level level) noexcept : BaseLogger(level) {}

void StdoutLogger::flush() {
    TryFlush(&std::cout);
}

void StdoutLogger::log(const std::string &msg, Level lvl) {
    if (lvl >= level())
        std::cout << get_level_prefix(lvl) << msg << std::endl;
}
