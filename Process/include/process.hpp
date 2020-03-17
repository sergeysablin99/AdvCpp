#ifndef PROCESS_H
#define PROCESS_H

#include <string>


class Process
{
public:
    explicit  Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;
    void closeStdin();

    void close();

private:
    // pipe_p - родитель пишет в 1-й элемент, читает со 2-го
    // pipe_с - наследник пишет в 1-й элемент, читает со 2-го
    int pipefd[2];
    int cpid;
};

#endif