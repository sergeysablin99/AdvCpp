//
// Created by Sergey Sablin on 17.04.2020.
// @serewkasablin@outlook.com
//

#include <chrono>
#include <iostream>
#include "mmap_allocator.h"
#include <unistd.h>
#include "shmap.h"
#include <string>
#include <sys/wait.h>
#include <thread>


int main() {
    using mapAlloc = shmem::MMapAllocator<std::pair<int, int>, std::pair<int, int>[1024]>;
    shmem::Shmap<int, int, std::less<>, mapAlloc> map;
    map.reset(1, 2);

    using strAlloc = shmem::MMapAllocator<char, char[1024]>;
    std::basic_string<char,  std::char_traits<char>, strAlloc> str{"Hello"};
    int child = fork();


    if (child) {
        map.reset(1, 2);
        str += ", world!";
    std::cout << str.c_str() << std::endl;
    }
    else {
        std::this_thread::sleep_for(std::chrono::seconds (10));
//        std::cout << map.at(1) << std::endl << str;
    std::cout << str.c_str() << std::endl;
    }

    waitpid(child, nullptr, 0);
    return 0;
}