//
// Created by Сергей on 25.03.2020.
//

#ifndef LOG_BASE_LOGGER_H
#define LOG_BASE_LOGGER_H

#include <ios>
#include <string>
#include <utility>

namespace logger {
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class LoggerException : public std::exception {
    private:
        std::string msg_;
    public:
        explicit LoggerException(std::string message);
        const char* what() const noexcept override { return msg_.c_str(); };
    };

    class BaseLogger {
    public:
        explicit BaseLogger(Level level = Level::INFO) noexcept;

        void debug(const std::string& msg);
        void info(const std::string& msg);
        void warn(const std::string& msg);
        void error(const std::string& msg);
        void set_level(Level level) noexcept;
        Level level() const noexcept;
        virtual void flush() = 0;

        virtual ~BaseLogger() = default;
    private:
        Level level_;
        virtual void log(const std::string& msg, Level) = 0;

    protected:
        std::string get_level_prefix(Level level);

        template<typename T>
        void TryFlush(std::basic_ostream<T>* stream)
        {
            try {
                stream->flush();
            }
            catch (const std::ios_base::failure& e) {
                throw LoggerException(e.what());
            }
            if (!stream->good()) {
                stream->clear();
                throw LoggerException("Bad flush");
            }
        };
    };
}

#endif //LOG_BASE_LOGGER_H
