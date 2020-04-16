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
    src_addr_(src_addr), src_port_(src_port),
    read_pos_(0), write_pos_(0),
    write_finished_(true), read_finished_(true)
    {}

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

Connection::Connection(const std::string& addr, int port):
    read_pos_(0), write_pos_(0),
    write_finished_(true), read_finished_(true)
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
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

void Connection::write(std::shared_ptr<void> data, size_t size)
{
    if (!is_opened())
        throw WriteError("Connection is closed");

    write_data_ = reinterpret_cast<char*>(data.get());
    write_size_ = size;
    write();
    write_finished_ = false;
}

void Connection::read(std::shared_ptr<void> data, size_t size)
{
    if (!is_opened())
        throw ReadError("Connection is closed");

    read_data_ = reinterpret_cast<char*>(data.get());
    read_size_ = size;
    read();
    read_finished_ = false;
}

Connection::Connection(int fd) :
    fd_(fd), opened_(true),
    read_pos_(0), write_pos_(0),
    write_finished_(true), read_finished_(true)
{}

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

void Connection::read() {
    ssize_t reply = ::read(fd_, read_data_ + read_pos_, read_size_);
    if (errno == EAGAIN || errno == EWOULDBLOCK)
        return;
    if (reply == -1)
        throw ReadError(std::to_string(errno));
    read_pos_ += reply;
    if (read_pos_ == read_size_)
    {
        read_pos_ = 0;
        read_finished_ = true;
    }
}

void Connection::write() {
    ssize_t reply = ::write(fd_, write_data_ + write_pos_, write_size_);
    if (errno == EAGAIN || errno == EWOULDBLOCK)
        return;
    if (reply == -1)
        throw WriteError(std::to_string(errno));
    write_pos_ += reply;
    if (write_pos_ == write_size_)
    {
        write_pos_ = 0;
        write_finished_ = true;
    }
}

bool Connection::read_finished() const noexcept {
    return read_finished_;
}

bool Connection::write_finished() const noexcept {
    return write_finished_;
}
