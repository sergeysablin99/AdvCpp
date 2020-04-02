//
// Created by Сергей on 26.03.2020.
//

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H

#include "base_logger.h"
#include <memory>

namespace logger {
    class Logger {
    private:
        std::unique_ptr<BaseLogger> global_logger_;

        Logger();

    public:
        Logger(Logger &other) = delete;

        void operator=(const Logger &) = delete;

        static Logger &get_instance();

        BaseLogger* get_global_logger();

        void set_global_logger(std::unique_ptr<BaseLogger> logger);
    };

    std::unique_ptr<BaseLogger> create_file_logger(const std::string &path, logger::Level level);

    std::unique_ptr<BaseLogger> create_stdout_logger(logger::Level level);

    std::unique_ptr<BaseLogger> create_stderr_logger(logger::Level level);

    void debug(const std::string &msg);

    void info(const std::string &msg);

    void warning(const std::string &msg);

    void error(const std::string &msg);
}
#endif //LOG_LOGGER_H
