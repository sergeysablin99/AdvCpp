//
// Created by gtl on 29.03.2020.
//

#include <arpa/inet.h>
#include "connection.h"
#include <cstring>
#include "exceptions.h"
#include <unistd.h>

using namespace tcp;

Connection::Connection(int fd, in_addr addr, in_port_t port,
                       in_addr src_addr, in_port_t src_port) :
    fd_(fd), dst_addr_(addr), dst_port_(port), opened_(true),
    src_addr_(src_addr), src_port_(src_port)
    {
}

void Connection::connect(const std::string& addr, int port)
{
    dst_addr_.s_addr = inet_addr(addr.data());
    dst_port_ = htons(port);

    sockaddr_in dst{};
    dst.sin_family = AF_INET;
    dst.sin_port = dst_port_;
    dst.sin_addr = dst_addr_;
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&dst), sizeof(dst)) == -1)
        throw ConnectError(std::strerror(errno));
    opened_ = true;
}

Connection::Connection(const std::string& addr, int port)
{
    fd_ = fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
        throw SocketError(std::strerror(errno));
    connect(addr, port);
}

bool Connection::is_opened() const noexcept
{
    return opened_;
}

void Connection::close()
{
    if (is_opened())
        if (fd_ != -1) {
            if (::close(fd_) == -1)
               throw CloseError(std::strerror(errno));
            opened_ = false;
        }
}

ssize_t Connection::write(const void* data, size_t size) const
{
    if (!is_opened())
        throw WriteError("Connection is closed");
    ssize_t reply = ::write(fd_, data, size);
    if (reply == -1)
        throw WriteError(std::strerror(errno));
    return reply;
}

ssize_t Connection::read(void *data, size_t size) const
{
    if (!is_opened())
        throw ReadError("Connection is closed");
    ssize_t reply = ::read(fd_, data, size);
    if (reply == -1)
        throw ReadError(std::strerror(errno));
    return reply;
}

Connection::Connection(int fd) : fd_(fd), opened_(true) {
}

Connection::~Connection() {
    try {
        close();
    }
    catch (tcp::TcpError& e) {

    }
}

void Connection::ExtractFd() {
    fd_ = -1;
}




