#include "../include/process.hpp"
#include <unistd.h>
#include <sys/wait.h>

Process::Process(const std::string &path)
{
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {  // Открыт дочерний процесс
    dup2(pipefd[0], STDIN_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    if (execl(path.data(), NULL) == -1) {
        perror("execl");
        exit(EXIT_FAILURE);
    }
        // что-то делает
        close();
    }
    else {  // Открыт родительский процесс
        // что-то делает
        close();
    }
}

Process::~Process() {
    ::kill(cpid, 0);
    ::waitpid(cpid, NULL, 0);
    exit((EXIT_SUCCESS));
}

size_t Process::write(const void *data, size_t len) {
    return ::write(pipefd[0], data, len);
}

void Process::writeExact(const void *data, size_t len) {
    int bytes_to_write = len;
    while(bytes_to_write) {
        bytes_to_write = bytes_to_write - write(data, bytes_to_write);
    }
}

size_t Process::read(void *data, size_t len) {
    return ::read(pipefd[1], data, len);
}

void Process::readExact(void *data, size_t len) {
    int bytes_to_read = len;
    while(bytes_to_read)
        bytes_to_read = bytes_to_read - read(data, bytes_to_read);
}

void Process::close() {
    ::close(pipefd[0]);
    ::close(pipefd[1]);
}