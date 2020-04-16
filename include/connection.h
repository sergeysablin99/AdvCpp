//
// Created by gtl on 29.03.2020.
//

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <memory>
#include <netinet/in.h>
#include <vector>
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
        bool opened_;

        bool read_finished_;
        bool write_finished_;
        char* read_data_;
        char* write_data_;
        size_t read_size_;
        size_t write_size_;
        int read_pos_;
        int write_pos_;

        Connection(int fd, in_addr addr, in_port_t port,
                    in_addr src_addr, in_port_t src_port);
        Connection(int fd);

        void ExtractFd();
        void read();
        void write();

        friend Server;
    public:
        Connection(const std::string& addr, int port);
        ~Connection();
        void write(std::shared_ptr<void> data, size_t size);
        void read(std::shared_ptr<void> data, size_t size);
        void close();
        bool is_opened() const noexcept;
        bool read_finished() const noexcept;
        bool write_finished() const noexcept;
        void connect(const std::string& addr, int port);
    };
}

#endif //TCP_CONNECTION_H
