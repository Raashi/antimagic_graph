#ifndef ANTIMAGIC_THREADS_H
#define ANTIMAGIC_THREADS_H

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

struct Mutex {
#ifdef _WIN32
    CRITICAL_SECTION cs{};
#else
    pthread_mutex_t mutex;
#endif

    Mutex ();
    ~Mutex();

    void lock();
    void unlock();
};

#endif //ANTIMAGIC_THREADS_H