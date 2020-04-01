//
// Created by gtl on 29.03.2020.
//

#include "connection.h"
#include <cstring>
#include "exceptions.h"
#include <unistd.h>

using namespace tcp;

Connection::Connection(int fd, in_addr addr, in_port_t port,
                       in_addr src_addr, in_port_t src_port) :
    fd_(fd), dst_addr(addr), dst_port(port), opened(false),
    src_addr(src_addr), src_port(src_port)
    {
    set_timeout(1);
}

void Connection::connect(in_addr addr, in_port_t port) {
    sockaddr_in dst{};
    dst.sin_family = AF_INET;
    dst.sin_port = port;
    dst.sin_addr = addr;
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&dst), sizeof(dst)) == -1)
        throw ConnectError(std::strerror(errno));
    opened = true;
}

Connection::Connection(in_addr ip, in_port_t port) :
    dst_addr(ip), dst_port(port), opened(false){
    fd_ = fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
        throw SocketError(std::strerror(errno));
    set_timeout(1);
    connect(ip, port);
}

bool Connection::is_opened() noexcept {
    return opened;
}

void Connection::set_timeout(int sec) {
    timeout.tv_sec = sec;
    timeout.tv_usec = 0;
    if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
        throw SetTimeoutError(std::strerror(errno));
}

void Connection::close() {
    if (is_opened()) {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        opened = false;
    }
}

ssize_t Connection::write(const void* data, size_t size) const {
    ssize_t reply = ::write(fd_, data, size);
    if (reply == -1)
        throw WriteError(std::strerror(errno));
    return reply;
}

ssize_t Connection::read(const void *data, size_t size) const {
    ssize_t reply = ::write(fd_, data, size);
    if (reply == -1)
        throw ReadError(std::strerror(errno));
    return reply;
}

void Connection::writeExact(const void *data, size_t len) const {
    size_t bytes_to_write = len;
    while(bytes_to_write > 0) {
        bytes_to_write -= write(data, bytes_to_write);
    }
}

void Connection::readExact(const void *data, size_t len) const {
    size_t bytes_to_read = len;
    while(bytes_to_read > 0)
        bytes_to_read -= read(data, bytes_to_read);
}




