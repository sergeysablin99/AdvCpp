//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_FILE_LOGGER_H
#define LOG_FILE_LOGGER_H

#include <base_logger.h>
#include <fstream>

class FileLogger : public BaseLogger {
public:
    explicit FileLogger(const std::string& path, Level level);
    explicit FileLogger(std::ofstream&& other, Level level);
    ~FileLogger() noexcept;
    void flush() override;

private:
    std::ofstream output;
    void log(const std::string& msg, Level) final;
};


#endif //LOG_FILE_LOGGER_H
