//
// Created by gtl on 29.03.2020.
//

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <netinet/in.h>

namespace tcp {
    class Server;

    class Connection {
    private:
        int fd_;
        in_addr dst_addr;
        in_addr src_addr;
        in_port_t src_port;
        in_port_t dst_port;
        timeval timeout;
        bool opened;
        Connection(int fd, in_addr addr, in_port_t port,
                    in_addr src_addr, in_port_t src_port);
        friend Server;

    public:
        Connection(in_addr ip, in_port_t port);

        ssize_t write(const void* data, size_t size) const;
        ssize_t read(void* data, size_t size) const;
        void close();
        bool is_opened() const noexcept;
        void readExact(void* data, size_t size) const;
        void writeExact(const void* data, size_t size) const;
        void set_timeout(int sec);
        void connect(in_addr addr, in_port_t port);
    };
}

#endif //TCP_CONNECTION_H
