cmake_minimum_required(VERSION 3.10)
project(Process)

set(CMAKE_CXX_STANDARD 14)

add_library(proc STATIC source/process.cpp)
target_include_directories(proc PUBLIC include)

add_executable(process source/main.cpp)
target_link_libraries(process proc)