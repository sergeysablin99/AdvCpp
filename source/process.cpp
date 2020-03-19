#include "../include/process.hpp"
#include <unistd.h>
#include <sys/wait.h>

Process::Process(const std::string &path)
{
    if (pipe(pipefd_pc) == -1) {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd_cp) == -1) {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {
        // Открыт дочерний процесс
        int reply = ::close(pipefd_pc[1]);
        if (reply == -1)
            perror("Close error");
        reply = ::close(pipefd_cp[0]);
        if (reply == -1)
            perror("Close error");

        dup2(pipefd_pc[0], STDIN_FILENO);
        dup2(pipefd_cp[1], STDOUT_FILENO);

        if (execl(path.data(), NULL) == -1) {
            perror("Execl error");
            exit(EXIT_FAILURE);
        }
    }
    else {
        // Открыт родительский процесс
        int reply = ::close(pipefd_pc[0]);
        if (reply == -1)
            perror("Close error");
        reply = ::close(pipefd_cp[1]);
        if (reply == -1)
            perror("Close error");

        char a[30] = "12345678901234567890";
        int c = write(a, sizeof(a));
        c = read(a, sizeof(a));

        closeStdin();
        c =  write(a, sizeof(a));
    }
}

Process::~Process() {
    close();
}

size_t Process::write(const void *data, size_t len) {
    int reply = ::write(pipefd_pc[1], data, len);
     if( reply == -1)
        perror("Write error");
      return reply;
}

void Process::writeExact(const void *data, size_t len) {
    int bytes_to_write = len;
    while(bytes_to_write) {
        bytes_to_write = bytes_to_write - write(data, bytes_to_write);
    }
}

size_t Process::read(void *data, size_t len) {
    int reply = ::read(pipefd_cp[0], data, len);
    if (reply == -1)
        perror("Read error");
    return reply;
}

void Process::readExact(void *data, size_t len) {
    int bytes_to_read = len;
    while(bytes_to_read)
        bytes_to_read = bytes_to_read - read(data, bytes_to_read);
}

void Process::close() {
    size_t reply;
    if (isReadable()) {
        reply = ::close(pipefd_cp[0]);
        if (reply == -1)
            perror("Close error");
        reply = ::close(pipefd_pc[1]);
        if (reply == -1)
            perror("Kill error");
    }
    reply = ::waitpid(cpid, NULL, 0);
    if (reply == -1)
        perror("Waitpid error");
    exit((EXIT_SUCCESS));
}

void Process::closeStdin() {
    int reply = ::close(pipefd_cp[0]);
    if (reply == -1)
        perror("Close error");
    reply = ::close(pipefd_pc[1]);
    if (reply == -1)
        perror("Close error");
}

bool Process::isReadable() const {
    char a;
    char b[5] = "test";
    return ::read(pipefd_cp[0], &a, 1) != -1 and ::write(pipefd_pc[1], b, sizeof(b)) != -1;
}