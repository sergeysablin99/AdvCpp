//
// Created by Сергей on 25.03.2020.
//

#include "file_logger.h"

using namespace logger;

FileLogger::FileLogger(const std::string &path, Level level) : BaseLogger(level) {
    output.open(path);
    if (!output.is_open())
        throw LoggerException("Can't open file");
}

FileLogger::FileLogger(std::ofstream&& other, Level level) : BaseLogger(level), output(std::move(other)) {}

FileLogger::~FileLogger() noexcept {
    output.close();
    if (!output.good())
        log("Can't close log file", Level::WARNING);
}

void FileLogger::flush() {
    TryFlush(&output);
}

void FileLogger::log(const std::string &msg, Level lvl) {
    if (lvl >= level())
        output << get_level_prefix(lvl) << msg << std::endl;
}