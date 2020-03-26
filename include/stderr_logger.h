//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_STDERR_LOGGER_H
#define LOG_STDERR_LOGGER_H

#include <base_logger.h>

class StderrLogger : public BaseLogger {
public:
    explicit StderrLogger( Level level) noexcept;
    ~StderrLogger() noexcept = default;
    void flush() override;

private:
    void log(const std::string& msg, Level) final;
};

#endif //LOG_STDERR_LOGGER_H
