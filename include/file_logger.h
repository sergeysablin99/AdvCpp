//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_FILE_LOGGER_H
#define LOG_FILE_LOGGER_H

#include "base_logger.h"
#include <fstream>

namespace logger {
    class FileLogger : public logger::BaseLogger {
    public:
        FileLogger(const std::string& path, logger::Level level);
        FileLogger(std::ofstream&& other, logger::Level level);
        ~FileLogger() noexcept;
        void flush() override;

    private:
        std::ofstream output;
        void log(const std::string& msg, logger::Level) final;
    };
}

#endif //LOG_FILE_LOGGER_H
