//
// Created by gtl on 28.03.2020.
//

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "connection.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <utility>

namespace tcp {
    class Server {
    private:
        int fd_;
        bool opened;
        int connections;
        in_addr addr_;
        in_port_t port_;

    public:
        Server(in_addr ip, in_port_t port);
        Connection accept();
        void close();
        bool is_opened() noexcept;
        void open(in_addr ip, in_port_t port);
        void set_max_connect(int count);
    };
}

#endif //TCP_SERVER_H
