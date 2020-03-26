#include <stdout_logger.h>

int main() {
    StdoutLogger logger(StdoutLogger::Level::ERROR);
    logger.debug("Debug msg");
    logger.set_level(StdoutLogger::Level::WARNING);
    logger.info("Info msg");
    logger.warn("Warn msg");
    logger.error("Error msg");
    return 0;
}
