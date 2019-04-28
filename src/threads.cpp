#include <iostream>
#include <time.h>
#include <iterator>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
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
    DeleteCriticalSection(&this->cs);
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

void exit_thread(int ret_val) {
#ifdef _WIN32
    _endthreadex(ret_val);
#else
    pthread_exit(new int(ret_val));
#endif
}

ThreadPull::ThreadPull(int argc, char **argv, ifstream* fp) {
    this->fp = fp;

    this->thread_count = get_arg(argc, argv, "-tc", DEFAULT_THREAD_COUNT);
    cout << SYS_MSG << "Thread count set to " << thread_count << endl;

    this->skip = has_arg(argc, argv, "-skip");
    this->skip_time = get_arg(argc, argv, "-skt", DEFAULT_SKIP_TIME);
    this->vec_skipped = vector<string>();

    if (this->skip)
        cout << SYS_MSG << "Maximum time for calculating graph set to "
             << this->skip_time << " seconds" << endl;
}

uint main_worker(void* arg) {
    auto * wa = (WorkerArg*) arg;

    string line;

    do {
        bool leave = false;
        wa->tp->mutex.lock();
        if (not (bool) getline(*(wa->tp->fp), line))
            leave = true;
        wa->tp->mutex.unlock();
        if (leave) {
            exit_thread(0);
            return 0;
        }

        uint ret_val = wa->worker(wa, line);

        wa->tp->data_read++;
        if (ret_val == WORKER_RETURN_OKAY)
            wa->tp->data_okay++;
        if (ret_val == WORKER_RETURN_SKIPPED) {
            wa->tp->mutex_skipped.lock();
            wa->tp->vec_skipped.push_back(line);
            wa->tp->mutex_skipped.unlock();
            wa->tp->data_skipped++;
        }

    } while (true);
}

#ifdef _WIN32
uint __stdcall windows_worker(void* arg) {
    return main_worker(arg);
}
#else
void* posix_worker(void* arg) {
    return new int(main_worker(arg));
}
#endif

void ThreadPull::run(worker_t worker, void* arg, worker_final_t worker_final) {
    time_t start, end;
    time(&start);

    WorkerArg worker_arg{this, arg, worker};

#ifdef _WIN32
    cout << SYS_MSG << "Launching windows threads..." << endl;
    HANDLE threads[this->thread_count];
    for (int i = 0; i < this->thread_count; ++i) {
        auto thread = (HANDLE) _beginthreadex(nullptr, 0, windows_worker, (void*) &worker_arg, 0, nullptr);
        threads[i] = thread;
    }

    WaitForMultipleObjects(this->thread_count, threads, true, INFINITE);
#else
    cout << SYS_MSG << "Launching POSIX threads..." << endl;
    pthread_t threads[this->thread_count];
    pthread_attr_t attrs[this->thread_count];

    for (int i = 0; i < this->thread_count; ++i) {
        pthread_attr_init(&attrs[i]);
        pthread_create(&threads[i], &attrs[i], posix_worker, (void *) &worker_arg);
    }

    for (int i = 0; i < this->thread_count; ++i) {
        pthread_join(threads[i], nullptr);
    }
#endif

    if (worker_final != nullptr)
        worker_final((void*) &worker_arg);

    time(&end);
    double elapsed = difftime(end, start);
    cout << SYS_MSG << "Elapsed time: " << elapsed / 60 << " minutes" << endl;

    this->write_skipped();
}

void ThreadPull::write_skipped() {
    if (!this->skip || this->vec_skipped.empty())
        return;
    write_to_file("skipped.txt", this->vec_skipped);
}

WorkerArg::WorkerArg(ThreadPull *tp, void *bp, worker_t worker) {
    this->tp = tp;
    this->bp = bp;
    this->worker = worker;
}
