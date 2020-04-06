#include <iostream>
#include "server.h"
#include <string>

int main() {
    tcp::Server server("127.0.0.1", htons(8888));
    auto connection = server.accept();
    char reply_str[5];
    connection.read(reply_str, sizeof(reply_str));  // echo hello | nc localhost 8888
    std::cout << reply_str;
    std::string request_str = "Bye";
    connection.write(&request_str, sizeof(request_str));
    return 0;
}
