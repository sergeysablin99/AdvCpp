#include <iostream>
#include <process.hpp>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        try {
            Process process(argv[1]);
        }
        catch (Process::Exception& except) {
            std::cout << except.what();
        }
        // Если программа дошла до этого места, то программа успешно запустилась и можно исполнять комманды
    }
    else {
        std::cout << "Error: no path provided";
    }
    return 0;
}
