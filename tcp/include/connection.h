//
// Created by gtl on 29.03.2020.
//

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <netinet/in.h>
#include <string>

namespace tcp {
    class Server;

    class Connection {
    private:
        int fd_;
        in_addr dst_addr_;
        in_addr src_addr_;
        in_port_t src_port_;
        in_port_t dst_port_;
        timeval timeout_;
        bool opened_;
        Connection(int fd, in_addr addr, in_port_t port,
                    in_addr src_addr, in_port_t src_port);
        friend Server;

    public:
        Connection(const std::string& addr, int port);

        ssize_t write(const void* data, size_t size) const;
        ssize_t read(void* data, size_t size) const;
        void close();
        bool is_opened() const noexcept;
        void readExact(void* data, size_t size) const;
        void writeExact(const void* data, size_t size) const;
        void set_timeout(int sec);
        void connect(const std::string& addr, int port);
    };
}

#endif //TCP_CONNECTION_H
