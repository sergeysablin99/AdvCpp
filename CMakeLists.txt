cmake_minimum_required(VERSION 3.15)
project(log)

set(CMAKE_CXX_STANDARD 14)

add_library(base_logger STATIC source/base_logger.cpp)
add_library(stderr_logger STATIC source/stderr_logger.cpp)
add_library(stdout_logger STATIC source/stdout_logger.cpp)
add_library(file_logger source/file_logger.cpp)
add_library(logger source/logger.cpp)
target_include_directories(base_logger PUBLIC include)

target_link_libraries(stderr_logger base_logger)
target_link_libraries(stdout_logger base_logger)
target_link_libraries(file_logger base_logger)

target_link_libraries(logger stderr_logger stdout_logger file_logger)

add_executable(log_main source/main.cpp)
target_link_libraries(log_main logger)

