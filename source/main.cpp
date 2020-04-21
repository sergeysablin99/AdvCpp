#include <fstream>
#include <iostream>
#include "server.h"
#include <string>

int main() {
    std::string filename;
    size_t filesize;
    std::cin >> filename >> filesize;
    std::ifstream input(filename);
    char* file_data = new char[filesize];
    input.read(file_data, filesize);

    auto write_data = std::shared_ptr<char>(file_data);

    auto read_data = std::make_shared<char>(5);
    auto read_size = 5;
    auto handler = [&write_data, &filesize, &read_data, &read_size](tcp::Connection& c)
            {
                c.read(read_data, read_size);
                if (c.read_finished())
                    std::cout << "Read finished" << std::endl;
                c.write(write_data, filesize);
                std::cout << "Writting..." << std::endl;
                if (c.write_finished()) {
                    std::cout << "Write finished" << std::endl;
                    c.close();
                }
            };
    tcp::Server server("127.0.0.1", 8088, handler);
    server.handleClients();  // echo 12345 | nc localhost 8088
    return 0;
}
