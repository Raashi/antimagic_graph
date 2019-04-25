#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "threads.h"

Mutex::Mutex() {
#ifdef _WIN32
    InitializeCriticalSection(&this->cs);
#else
    pthread_mutex_init(&this->mutex, nullptr);
#endif
}

Mutex::~Mutex() {
#ifdef _WIN32
    DeleteCriticalSection(this->cs);
#else
    pthread_mutex_destroy(&this->mutex);
#endif
}

void Mutex::lock() {
#ifdef _WIN32
    EnterCriticalSection(&this->cs);
#else
    pthread_mutex_lock(&this->mutex);
#endif
}

void Mutex::unlock() {
#ifdef _WIN32
    LeaveCriticalSection(&this->cs);
#else
    pthread_mutex_unlock(&this->mutex);
#endif
}
