#ifndef ANTIMAGIC_THREADS_H
#define ANTIMAGIC_THREADS_H

#include <atomic>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "utils.h"

using namespace std;

typedef uint (*worker_t)(void*, string);
typedef void (*worker_final_t)(void*);

const uint WORKER_RETURN_OKAY = 0;

const ulong  DEFAULT_THREAD_COUNT = 4;


struct Mutex {
#ifdef _WIN32
    CRITICAL_SECTION cs{};
#else
    pthread_mutex_t mutex{};
#endif

    Mutex ();
    ~Mutex();

    void lock();
    void unlock();
};

void exit_thread(int);

struct ThreadPull {
    atomic_int data_read{0};
    atomic_int data_okay{0};
    atomic_int data_skipped{0};

    Mutex mutex;

    ifstream* fp;
    ulong thread_count;

    ThreadPull(int, char**, ifstream*);

    void run(worker_t, void*, worker_final_t=nullptr);
};

struct WorkerArg {
    void* bp;
    ThreadPull* tp;
    worker_t worker;

    WorkerArg(ThreadPull* tp, void* bp, worker_t worker);
};

#endif //ANTIMAGIC_THREADS_H