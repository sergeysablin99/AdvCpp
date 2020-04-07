//
// Created by gtl on 28.03.2020.
//


#include <arpa/inet.h>
#include "connection.h"
#include <cstring>
#include <sys/epoll.h>
#include "exceptions.h"
#include <unistd.h>
#include "server.h"

using namespace tcp;

Server::Server(const std::string&  addr, int port,
        std::function<void(Connection&)> callback, size_t conn) :
        callback_(std::move(callback)), opened_(false)
{
    open(addr, port, conn);
}

bool Server::is_opened() noexcept
{
    return opened_;
}

void Server::close()
{
    if (is_opened()) {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        opened_ = false;
    }
}

void Server::open(const std::string& ip, int port, size_t conn)
{
    addr_.s_addr = inet_addr(ip.data());
    port_ = htons(port);
    connections_ = conn;

    if (is_opened())
        throw OpenError("Server is already opened, close first, then re-open");

    fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd_ == -1)
        throw SocketError(std::strerror(errno));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = port_;
    addr.sin_addr = addr_;

    if (bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        throw BindError(std::strerror(errno));
    }

    if (listen(fd_, connections_) == -1)
    {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        throw ListenError(std::strerror(errno));
    }

    epoll_ = epoll_create(1);
    if (epoll_ == -1)
    {
        if (::close(fd_) == -1)
            throw CloseError(std::strerror(errno));
        throw SocketError(std::strerror(errno));
    }

    addEvent(fd_, EPOLLIN);
    opened_ = true;
}

void Server::acceptClients()
{
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    int fd{};

    while(true) {
        fd = accept4(fd_, reinterpret_cast<sockaddr*>(&addr), &len, SOCK_NONBLOCK);
        if (fd < 0) {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
                return;
            else
                throw RuntimeError(std::strerror(errno));
        }

        addEvent(fd, EPOLLIN);
    }
}


void Server::addEvent(int fd, uint32_t events)
{
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epoll_, EPOLL_CTL_ADD, fd, &event) < 0)
        throw RuntimeError(std::strerror(errno));
}

void Server::handleClients()
{
    epoll_event events[connections_];

    while(true) {
        int nfds = epoll_wait(epoll_, events, connections_, -1);
        if (nfds < 0)
            if (errno == EINTR)
                continue;
            else
                throw RuntimeError(std::strerror(errno));

        for (int i{}; i < nfds; ++i)
        {
            int fd = events[i].data.fd;
            auto event = events[i].events;

            if (fd == fd_)
                acceptClients();
            else {
                handleConnection(fd, event);
            }
        }
    }
}

void Server::handleConnection(int fd, uint32_t event)
{
    if (event & EPOLLHUP || event & EPOLLERR)
    {
        if (::close(fd) == -1)
            throw CloseError(std::strerror(errno));
        return;
    }


    Connection c(fd);
    callback_(c);
}


