#ifndef ANTIMAGIC_THREADS_H
#define ANTIMAGIC_THREADS_H

#include <atomic>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

#include "utils.h"
#include "brute.h"

using namespace std;


const uint DEFAULT_THREAD_COUNT = 4;

struct ThreadPull {
    Mutex mutex;
    Mutex mutex_progress;

    ifstream fp;
    uint thread_count;

    ulong graph_count;
    ulong graph_print_count;
    atomic_long graph_read_count{0};
    int progress{0};
    AntimagicBruteParams abp;

    ThreadPull(uint, char *);

    ~ThreadPull();

    void run();
};

#endif //ANTIMAGIC_THREADS_H