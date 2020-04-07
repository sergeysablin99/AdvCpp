//
// Created by gtl on 28.03.2020.
//


#include <arpa/inet.h>
#include "connection.h"
#include <cstring>
#include "exceptions.h"
#include <unistd.h>
#include "server.h"

using namespace tcp;

Server::Server(const std::string&  ip, int port) :
    opened(true), connections(1), port_(htons(port)),
    addr_({.s_addr = inet_addr(ip.data())})
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
        throw SocketError(std::strerror(errno));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = port_;
    addr.sin_addr = addr_;

    if (bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        close();
        throw BindError(std::strerror(errno));
    }

    if (listen(fd_, connections) == -1) {
        close();
        throw ListenError(std::strerror(errno));
    }
}

bool Server::is_opened() noexcept
{
    return opened;
}

void Server::close()
{
    if (is_opened()) {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        opened = false;
    }
}

void Server::set_max_connect(size_t count)
{
    connections = count;
    if (listen(fd_, connections) == -1)
        throw ListenError(std::strerror(errno));
}

void Server::open(in_addr ip, in_port_t port)
{
    if (is_opened())
        throw OpenError("Server is already opened, close first, then re-open");

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
        throw SocketError(std::strerror(errno));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr = ip;

    if (bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
        throw BindError(std::strerror(errno));

    if (listen(fd_, connections) == -1)
        throw ListenError(std::strerror(errno));
}

Connection Server::accept()
{
    if (!is_opened())
        throw AcceptError("Server isn't opened, open first, then accept");

    sockaddr_in peer_addr{};
    socklen_t peer_addr_size = sizeof(peer_addr);

    int cfd = ::accept(fd_, reinterpret_cast<sockaddr*>(&peer_addr), &peer_addr_size);
    if (cfd == -1)
        throw AcceptError(std::strerror(errno));

    return {cfd, peer_addr.sin_addr, peer_addr.sin_port, addr_, port_};
}
