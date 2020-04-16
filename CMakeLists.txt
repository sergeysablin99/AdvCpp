cmake_minimum_required(VERSION 3.15)
project(tcp)

set(CMAKE_CXX_STANDARD 14)

add_library(tcp STATIC source/server.cpp source/connection.cpp)
target_include_directories(tcp PUBLIC include)

add_executable(tcp_main source/main.cpp)
target_link_libraries(tcp_main tcp)
