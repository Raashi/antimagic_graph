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


const uint   WORKER_RETURN_OKAY   = 0;
const ulong  DEFAULT_THREAD_COUNT = 4;

struct ThreadPull {
    atomic_int data_read{0};
    atomic_int data_okay{0};

    Mutex mutex;

    ifstream* fp;
    ulong thread_count;

    AntimagicBruteParams abp;

    ThreadPull(int, char**, ifstream*);
    void run();
};

#endif //ANTIMAGIC_THREADS_H