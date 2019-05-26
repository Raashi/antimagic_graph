#include <iostream>
#include <time.h>
#include <iterator>
#include <thread>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

#include "threads.h"

void exit_thread(int ret_val) {
#ifdef _WIN32
    _endthreadex(ret_val);
#else
    pthread_exit(new int(ret_val));
#endif
}

ThreadPull::ThreadPull(uint thread_count, char *filename) {
    fp = ifstream(filename);
    graph_count = get_graph_count(filename);
    graph_print_count = max(graph_count / 100, 10ul);
    this->thread_count = thread_count;
    cout << SYS_MSG << "Thread count set to " << thread_count << endl;
}

ThreadPull::~ThreadPull() {
    fp.close();
}

uint main_worker(void *arg) {
    auto *thread_pull = (ThreadPull *) arg;
    AntimagicBruteParams *params = &(thread_pull->abp);

    string line;

    do {
        bool leave = false;
        thread_pull->mutex.lock();
        if (not(bool) getline(thread_pull->fp, line))
            leave = true;
        thread_pull->mutex.unlock();
        if (leave) {
            exit_thread(0);
            return 0;
        }

        params->worker(line);

        thread_pull->graph_read_count++;
        if (thread_pull->graph_read_count % thread_pull->graph_print_count == 0) {
            double graph_read_count = (long) thread_pull->graph_read_count;
            double graph_count = 1.0 / (long) thread_pull->graph_count;
            int progress = (int) round(graph_read_count * graph_count * 100);
            thread_pull->mutex_progress.lock();
            if (progress <= thread_pull->progress) {
                thread_pull->mutex_progress.unlock();
                continue;
            }
            thread_pull->progress = progress;
            printf("\r%sProgress: %i%%", SYS_MSG.c_str(), progress);
            fflush(stdout);
            thread_pull->mutex_progress.unlock();
        }
    } while (true);
}

#ifdef _WIN32
uint __stdcall windows_worker(void* arg) {
    return main_worker(arg);
}
#else

void *posix_worker(void *arg) {
    return new int(main_worker(arg));
}

#endif

void ThreadPull::run() {
    time_t start, end;
    time(&start);

#ifdef _WIN32
    cout << SYS_MSG << "Launching windows threads..." << endl;
    cout << SYS_MSG << "Progress: 0%";
    HANDLE threads[this->thread_count];
    for (int i = 0; i < this->thread_count; ++i) {
        auto thread = (HANDLE) _beginthreadex(nullptr, 0, windows_worker, (void*) this, 0, nullptr);
        threads[i] = thread;
    }

    WaitForMultipleObjects(this->thread_count, threads, true, INFINITE);
#else
    cout << SYS_MSG << "Launching POSIX threads..." << endl;
    cout << SYS_MSG << "Process: 0%";
    pthread_t threads[this->thread_count];
    pthread_attr_t attrs[this->thread_count];

    for (int i = 0; i < this->thread_count; ++i) {
        pthread_attr_init(&attrs[i]);
        pthread_create(&threads[i], &attrs[i], posix_worker, (void *) this);
    }

    for (int i = 0; i < this->thread_count; ++i) {
        pthread_join(threads[i], nullptr);
    }
#endif

    printf("\r%sProgress: 100%%\n", SYS_MSG.c_str());
    abp.finalize();

    time(&end);
    double elapsed = difftime(end, start);
    cout << SYS_MSG << "Elapsed time: " << setprecision(2) << elapsed / 60 << " minutes" << endl;
}
