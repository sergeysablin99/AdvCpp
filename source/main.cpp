#include <fstream>
#include <iostream>
#include "server.h"
#include <string>

int main() {
    std::ifstream input("../1mb.txt");
    char file_data[1'048'576];
    input.read(file_data, sizeof(file_data));

    char* write_data = file_data;
    size_t write_size = sizeof(file_data);

    auto handler = [&write_data, &write_size](tcp::Connection& c) {

        ssize_t bytes_written = c.write(write_data, write_size);

        write_data = write_data + bytes_written;
        write_size -= bytes_written;

        if (write_size == 0) {
            c.close();
            std::cout << "Succeed!" << std::endl;
        }
        else
            std::cout << write_size << std::endl;
    };
    tcp::Server server("127.0.0.1", 8080, handler);
    server.handleClients();  // nc localhost 8080 Hello
    return 0;
}
