#include <fstream>
#include <iostream>
#include "server.h"
#include <string>

int main() {
    std::ifstream input("../1mb.txt");
    char* file_data = new char[1024*1024];
    input.read(file_data, 1024*1024);

    auto write_data = std::shared_ptr<char>(file_data);
    size_t write_size = 1024*1024;

    auto read_data = std::shared_ptr<char>(new char[5]);
    auto read_size = 5;
    auto handler = [&write_data, &write_size, &read_data, &read_size](std::shared_ptr<tcp::Connection> c)
            {
        c->read(read_data, read_size);
        if (c->read_finished())
            std::cout << "Read finished" << std::endl;
        c->write(write_data, write_size);
        if (c->write_finished()) {
            std::cout << "Write finished" << std::endl;
            c->close();
        }
        };
    tcp::Server server("127.0.0.1", 8088, handler);
    server.handleClients();  // echo 12345 | nc localhost 8088
    return 0;
}
