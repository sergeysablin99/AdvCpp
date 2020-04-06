#include "logger.h"
#include <iostream>

int main() {
    logger::info("Info message");

    auto log = logger::create_stdout_logger(logger::Level::INFO);
    log->debug("Debug message");  // Это сообщение не выведется
    log->error("Error message");

    logger::Logger::get_instance().set_global_logger(std::move(log));
    logger::debug("Debug message");  // Это сообщение не выведется
    logger::info("Info message");

    try {
        logger::Logger::get_instance().set_global_logger(nullptr);  // Logger пуст
        logger::warning("Warning message");
    }
    catch (const logger::LoggerException& e) {
        std::cout << e.what();
    }

    return 0;
}
