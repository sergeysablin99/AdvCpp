cmake_minimum_required(VERSION 3.15)
project(shmem_example)

set(CMAKE_CXX_STANDARD 14)


add_executable(example example/main.cpp)
target_include_directories(example PUBLIC lib/include)
target_link_libraries(example pthread)
