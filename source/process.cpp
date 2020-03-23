#include <process.hpp>
#include <unistd.h>
#include <sys/wait.h>

Process::Process(const std::string &path)
{
    int pipefd_pc[2];
    int pipefd_cp[2];
    if (pipe(pipefd_pc) == -1)
        throw "Cannot open fd to write";
    if (pipe(pipefd_cp) == -1) {
        int reply = ::close(pipefd_pc[0]);
        if (reply == -1)
            perror("Close error");
        reply = ::close(pipefd_pc[1]);
        if (reply == -1)
            perror("Kill error");
        throw "Cannot open fd to read";
    }
    fd_read = pipefd_cp[0];
    fd_write = pipefd_pc[1];
    cpid = fork();
    if (cpid == -1)
        throw "Fork error";
    if (cpid == 0) {
        // Открыт дочерний процесс
        int reply = ::close(pipefd_pc[1]);
        if (reply == -1)
            perror("Error: child process cannot close parent fd for writing");
        reply = ::close(pipefd_cp[0]);
        if (reply == -1)
            perror("Error: child process cannot close parent fd for reading");

        dup2(pipefd_pc[0], STDIN_FILENO);
        dup2(pipefd_cp[1], STDOUT_FILENO);

        if (execl(path.data(), nullptr) == -1)
            throw "Cannot execute program";
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
        perror("Write error");
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
        perror("Read error");
    return reply;
}

void Process::readExact(void *data, size_t len) {
    size_t bytes_to_read = len;
    while(bytes_to_read)
        bytes_to_read -= read(data, bytes_to_read);
}

void Process::close() {
    int reply;
    if (isReadable()) {
        reply = ::close(fd_read);
        if (reply == -1)
            perror("Close error");
        reply = ::close(fd_write);
        if (reply == -1)
            perror("Close error");
    }
    kill();
}

void Process::kill() {
    ::kill(cpid, SIGTERM);
    pid_t reply = ::waitpid(cpid, nullptr, 0);
    if (reply == -1)
        perror("Waitpid error");
}

void Process::closeStdin() {
    int reply = ::close(fd_write);
    if (reply == -1)
        perror("Close error");
}

bool Process::isReadable() const {
    struct stat sb;
    return fstat(fd_read, &sb) != -1;
}