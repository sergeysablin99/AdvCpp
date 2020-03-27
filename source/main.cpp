#include "stdout_logger.h"

int main() {
    logger::StdoutLogger logger(logger::Level::ERROR);
    logger.debug("Debug msg");
    logger.set_level(logger::Level::WARNING);
    logger.info("Info msg");
    logger.warn("Warn msg");
    logger.error("Error msg");
    return 0;
}
