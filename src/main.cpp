#include <exception>

#include "threads.h"

using namespace std;


int main(int argc, char **argv) {
    if (argc < 2)
        throw runtime_error("Wrong arguments");

    ThreadPull(get_arg(argc, argv, "-tc", DEFAULT_THREAD_COUNT), argv[1]).run();

    return 0;
}
