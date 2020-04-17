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
    using mapAlloc = shmem::MMapAllocator<std::pair<int, int>>;
    shmem::Shmap<int, int, mapAlloc> map;
    map.reset(1, 2);
    int child = fork();
    if (child) {
        map.reset(1, 2);
    }
    else {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << map.at(1);
    }

    waitpid(child, nullptr, 0);
    return 0;
}