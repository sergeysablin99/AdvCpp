//
// Created by gtl on 29.03.2020.
//

#ifndef TCP_EXCEPTIONS_H
#define TCP_EXCEPTIONS_H


#include <exception>
#include <string>
#include <utility>

class TcpError : public std::exception {
private:
    std::string msg;

public:
    explicit TcpError(std::string message) : msg(std::move(message)){};
    const char * what() const noexcept  override { return msg.c_str(); };
};

class SocketError : public TcpError {
public:
    explicit SocketError(std::string message) : TcpError(std::move(message)) {};
};

class BindError : public TcpError {
public:
    explicit BindError(std::string message) : TcpError(std::move(message)) {};
};

class CloseError : public TcpError {
public:
    explicit CloseError(std::string message) : TcpError(std::move(message)) {};
};

class ListenError : public TcpError {
public:
    explicit ListenError(std::string message) : TcpError(std::move(message)) {};
};

class OpenError : public TcpError {
public:
    explicit OpenError(std::string message) : TcpError(std::move(message)) {};
};

class AcceptError : public TcpError {
public:
    explicit AcceptError(std::string message) : TcpError(std::move(message)) {};
};

class ConnectError : public TcpError {
public:
    explicit ConnectError(std::string message) : TcpError(std::move(message)) {};
};

class SetTimeoutError : public TcpError {
public:
    explicit SetTimeoutError(std::string message) : TcpError(std::move(message)) {};
};

class WriteError : public TcpError {
public:
    explicit WriteError(std::string message) : TcpError(std::move(message)) {};
};

class ReadError : public TcpError {
public:
    explicit ReadError(std::string message) : TcpError(std::move(message)) {};
};

#endif //TCP_EXCEPTIONS_H
