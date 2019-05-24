#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <exception>

#include "perms.h"
#include "graph.h"
#include "threads.h"
#include "brute.h"

using namespace std;


int main(int argc, char **argv) {
    if (argc < 2)
        throw runtime_error("Wrong arguments");

    ifstream file(argv[1]);

    ThreadPull tp{argc, argv, &file};
    AntimagicBruteParams abp;
    tp.run(worker_antimagic, (void*) &abp, worker_antimagic_finalize);
    file.close();

    return 0;
}
