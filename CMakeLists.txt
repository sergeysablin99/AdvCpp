cmake_minimum_required(VERSION 3.15)
project(log)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/include)
add_executable(example example/main.cpp)
add_subdirectory(lib)
target_link_libraries(example logger)

