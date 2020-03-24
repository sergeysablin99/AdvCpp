#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process
{
public:
    class Exception : public std::exception {
    private:
        char* msg;
    public:
        explicit Exception(char* message) : msg(message) {};
        std::string what() { return msg; };
    };

    explicit Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;
    void closeStdin();

    void close();

private:
    void kill();
    int fd_read;
    int fd_write;
    pid_t cpid;
    bool eof;
};

#endif