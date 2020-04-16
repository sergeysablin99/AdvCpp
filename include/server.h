//
// Created by gtl on 28.03.2020.
//

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "connection.h"
#include <functional>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <utility>

namespace tcp {
    class Server {
    private:
        int fd_;
        int epoll_;
        bool opened_;
        int connections_;
        std::function<void(std::shared_ptr<Connection>)> callback_;
        in_addr addr_;
        in_port_t port_;
        std::map<int, std::shared_ptr<Connection>> clients_;

        void close();
        void addEvent(int fd, uint32_t events);
        void acceptClients();
        void handleConnection(int fd, uint32_t event);

        void readClient(int fd);
        void writeClient(int fd);

    public:
        Server(const std::string& addr, int port,
                std::function<void(std::shared_ptr<Connection>)> callback, size_t conn = 128);
        bool is_opened() noexcept;
        void handleClients();
        void open(const std::string& ip, int port, size_t conn = 128);
    };
}

#endif //TCP_SERVER_H
