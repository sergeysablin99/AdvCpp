cmake_minimum_required(VERSION 3.15)
project(log)

set(CMAKE_CXX_STANDARD 14)

add_library(logger STATIC source/base_logger.cpp
        source/stderr_logger.cpp
        source/stdout_logger.cpp
        source/file_logger.cpp source/logger.cpp)
target_include_directories(logger PUBLIC include)
