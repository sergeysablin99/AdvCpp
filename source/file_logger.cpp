//
// Created by Сергей on 25.03.2020.
//

#include "file_logger.h"

FileLogger::FileLogger(const std::string &path, Level level) : BaseLogger(level) {
    output.open(path);
    if (!output.is_open()) {
        output.clear();
        throw LoggerException("Can't open file");
    }
}

FileLogger::FileLogger(std::ofstream&& other, Level level) : output(std::move(other)) {}

FileLogger::~FileLogger() noexcept {
    output.close();
    if (!output.good()) {
        output.clear();
        log("Can't close log file", Level::WARNING);
    }
}

void FileLogger::flush() {
    try {
        output.flush();
    }
    catch (const std::ios_base::failure& e) {
        throw LoggerException(e.what());
    }
    if (!output.good()) {
        output.clear();
        throw LoggerException("Bad flush");
    }
}

void FileLogger::log(const std::string &msg, BaseLogger::Level level) {
    if (level >= this->level()) {
        switch (level) {
            case Level::DEBUG :
                output << "DEBUG: ";
                break;
            case Level ::INFO :
                output << "INFO: ";
                break;
            case Level::WARNING :
                output << "WARNING: ";
                break;
            case Level::ERROR :
                output << "ERROR: ";
                break;
        }
        output << msg << std::endl;
    }
}