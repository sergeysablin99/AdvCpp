#include <errno.h>
#include <exception>
#include <process.hpp>
#include <string.h>

Process::Process(const std::string &path) : eof(true)
{
    int pipefd_pc[2];
    int pipefd_cp[2];
    if (pipe(pipefd_pc) == -1)
        throw Exception("Can't open fd to write");
    if (pipe(pipefd_cp) == -1) {
        ::close(pipefd_pc[0]);
        ::close(pipefd_pc[1]);
        throw Exception("Can't open fd to read");
    }
    fd_read = pipefd_cp[0];
    fd_write = pipefd_pc[1];
    cpid = fork();
    if (cpid == -1) {

        throw Exception("Fork error");
    }
    if (cpid == 0) {
        // Открыт дочерний процесс
        ::close(oioefd_pc[0]);
        ::close(pipefd_pc[1]);
        ::close(pipefd_cp[0]);
        ::close(pipefd_cp[1]);


        dup2(pipefd_pc[0], STDIN_FILENO);
        dup2(pipefd_cp[1], STDOUT_FILENO);

        if (execl(path.data(), nullptr) == -1)
            throw Exception(strerror(errno));
    }
    else {
        // Открыт родительский процесс
        // Конструктор завершен, можно выполнять команды
    }
}

Process::~Process() {
    close();
}

size_t Process::write(const void *data, size_t len) {
    ssize_t reply = ::write(fd_write, data, len);
    if( reply == -1)
        throw Exception(strerror(errno));
    return reply;
}

void Process::writeExact(const void *data, size_t len) {
    size_t bytes_to_write = len;
    while(bytes_to_write > 0) {
        bytes_to_write -= write(data, bytes_to_write);
    }
}

size_t Process::read(void *data, size_t len) {
    ssize_t reply = ::read(fd_read, data, len);
    if (reply == -1)
       throw Exception(strerror(errno));
    eof = (reply == 0);
    return reply;
}

void Process::readExact(void *data, size_t len) {
    size_t bytes_to_read = len;
    while(bytes_to_read > 0)
        bytes_to_read -= read(data, bytes_to_read);
}

void Process::close() {
    if (isReadable()) {
        ::close(fd_read);
        ::close(fd_write);
    }
    kill();
}

void Process::kill() {
    ::kill(cpid, SIGTERM);
    pid_t reply = ::waitpid(cpid, nullptr, 0);
    if (reply == -1)
        throw Exception(strerror(errno));
}

void Process::closeStdin() {
    int reply = ::close(fd_write);
}

bool Process::isReadable() const {
    return !eof;
}
