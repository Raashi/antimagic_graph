#include <exception>
#include <io.h>

#include "threads.h"

using namespace std;


bool file_exists(char *filename) {
    return (access(filename, F_OK) != -1);
}

int main(int argc, char **argv) {
    if (argc < 2)
        throw runtime_error("Wrong arguments");
    if (!file_exists(argv[1]))
        throw runtime_error("File doesn't exist");

    ThreadPull(get_arg(argc, argv, "-tc", DEFAULT_THREAD_COUNT), argv[1]).run();

    return 0;
}
