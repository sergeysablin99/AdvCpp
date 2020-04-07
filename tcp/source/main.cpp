#include <iostream>
#include "server.h"

int main() {
    auto handler = [](tcp::Connection& c){
        char data[6];
        c.read(data, sizeof(data));
        std::cout << data << std::endl;
    };
    tcp::Server server("127.0.0.1", 8080, handler);
    server.handleClients();  // nc localhost 8080 Hello
    return 0;
}
