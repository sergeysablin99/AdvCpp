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
    using StrAlloc = shmem::MMapAllocator<char, char[1024]>;
    using my_str = std::basic_string<char,  std::char_traits<char>, StrAlloc>;
    StrAlloc str_alloc;
    my_str str{str_alloc};

    using MapAlloc = shmem::MMapAllocator<std::pair<int, my_str>, std::pair<int, my_str>[1024]>;
    const MapAlloc mapAlloc;

    shmem::Shmap<int, my_str, std::less<char>, MapAlloc> map(mapAlloc);

    map.reset(1, "0");
    int child = fork();
    if (child) {
        for (int i = 0; i < 150; ++i)
        {
            auto value = map.at(1);
            value.push_back('1');
            map.reset(1, value);
        }
    }
    else {
        std::this_thread::sleep_for(std::chrono::seconds (5));
        std::cout << map.at(1) << std::endl << map.at(1).size();
    }

    waitpid(child, nullptr, 0);
    return 0;
}