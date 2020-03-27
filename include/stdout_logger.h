//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_STDOUT_LOGGER_H
#define LOG_STDOUT_LOGGER_H

#include "base_logger.h"

namespace logger {
    class StdoutLogger : public BaseLogger {
    public:
        explicit StdoutLogger(Level level) noexcept;

        ~StdoutLogger() noexcept = default;

        void flush() override;

    private:
        void log(const std::string &msg, Level) final;
    };
}

#endif //LOG_STDERR_LOGGER_H
