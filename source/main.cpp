#include <fstream>
#include <iostream>
#include "server.h"
#include <string>

int main() {
    std::ifstream input("../1mb.txt");
    char* file_data = new char[1'048'576];
    input.read(file_data, sizeof(file_data));

    char* write_data = file_data;
    size_t write_size = sizeof(file_data);
    auto handler = [&write_data, &write_size](std::shared_ptr<tcp::Connection> c) {
        c->write(std::shared_ptr<char>(write_data), write_size);
        if (c->write_finished())
            c->close();
    };
    tcp::Server server("127.0.0.1", 8080, handler);
    server.handleClients();  // nc localhost 8080 Hello
    return 0;
}
