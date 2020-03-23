#include <process.hpp>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        try {
            Process process(argv[1]);
        }
        catch (const char* error) {
            fwrite(error, 1, sizeof(error), stdout);
        }
        // Если программа дошла до этого места, то программа успешно запустилась и можно исполнять комманды
    }
    else {
        char buffer[24] = "Error: no path provided";
        fwrite(buffer, 1, sizeof(buffer), stdout);
    }
    return 0;
}
