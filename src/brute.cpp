#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <atomic>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif


#include "graph.h"
#include "brute.h"
#include "utils.h"

using namespace std;


struct BruteParams {
    atomic_int checked{0};
    atomic_int non_antimagic{0};

    bool skip;
    double skip_time;
    atomic_int skipped{0};
    vector<string> vec_skipped;

#ifdef _WIN32
    CRITICAL_SECTION cs{};
    CRITICAL_SECTION cs_skipped{};
    CRITICAL_SECTION cs_print{};
#else
    pthread_mutex_t cs;
    pthread_mutex_t cs_skipped;
    pthread_mutex_t cs_print;
#endif

    ifstream* fp;
    ulong thread_count;

    explicit BruteParams (int argc, char** argv, ifstream* fp) {
#ifdef _WIN32
        InitializeCriticalSection(&this->cs);
        InitializeCriticalSection(&this->cs_skipped);
        InitializeCriticalSection(&this->cs_print);
#else
        pthread_mutex_init(&this->cs, nullptr);
        pthread_mutex_init(&this->cs_skipped, nullptr);
        pthread_mutex_init(&this->cs_print, nullptr);
#endif

        this->fp = fp;

        this->thread_count = get_arg(argc, argv, "-tc", THREAD_COUNT_DEFAULT);
        cout << SYS_MSG << "Thread count set to " << thread_count << endl;

        this->skip = has_arg(argc, argv, "-skip");
        this->skip_time = get_arg(argc, argv, "-skt", MAX_ANTIMAGIC_CALCULATION_TIME);
        this->vec_skipped = vector<string>();

        if (this->skip)
            cout << SYS_MSG << "Maximum time for calculating graph set to "
                 << this->skip_time << " seconds" << endl;
    };

    ~BruteParams() {
#ifdef _WIN32

#else
    pthread_mutex_destroy(&this->cs);
    pthread_mutex_destroy(&this->cs_skipped);
    pthread_mutex_destroy(&this->cs_print);
#endif
    };

    void print_stat(bool same_line) {
        if (this->skip)
            printf("\rChecked: %i Non-antimagic: %i Skipped: %i%s",
                   (int) this->checked,
                   (int) this->non_antimagic,
                   (int) this->skipped,
                   same_line ? "" : "\n");
        else
            printf("\rChecked: %i Non-antimagic: %i%s",
                   (int) this->checked,
                   (int) this->non_antimagic,
                   same_line ? "" : "\n");
    };

    void write_skipped() {
        if (!skip || this->vec_skipped.empty())
            return;
        ofstream output_file("skipped.txt");
        ostream_iterator<string> output_iterator(output_file, "\n");
        copy(this->vec_skipped.begin(), this->vec_skipped.end(), output_iterator);
    }
};

#ifdef _WIN32
unsigned int __stdcall brute_worker(void * param) {
#else
void* brute_worker(void * param) {
#endif
    auto * bp = (BruteParams*) param;
    string line;

    do {
        bool leave = false;
#ifdef _WIN32
        EnterCriticalSection(&bp->cs);
#else
        pthread_mutex_lock(&bp->cs);
#endif
        if (not (bool) getline(*(bp->fp), line))
            leave = true;
#ifdef _WIN32
        LeaveCriticalSection(&bp->cs);
        if (leave) _endthreadex(0);
#else
        pthread_mutex_unlock(&bp->cs);
        if (leave) pthread_exit(nullptr);
#endif

        Graph g(line);
        int antimagic = g.is_antimagic(bp->skip, bp->skip_time);

        bp->checked++;
        if (antimagic == NON_ANTIMAGIC)
            bp->non_antimagic++;
        else if (antimagic == TIME_OVERFLOW) {
#ifdef _WIN32
            EnterCriticalSection(&bp->cs_skipped);
#else
            pthread_mutex_lock(&bp->cs_skipped);
#endif
            bp->vec_skipped.push_back(line);
#ifdef _WIN32
            LeaveCriticalSection(&bp->cs_skipped);
#else
            pthread_mutex_unlock(&bp->cs_skipped);
#endif
            bp->skipped++;
        }

        if (bp->checked % 10 == 0) {
#ifdef _WIN32
            EnterCriticalSection(&bp->cs_print);
#else
            pthread_mutex_lock(&bp->cs_print);
#endif
            bp->print_stat(true);
            fflush(stdout);
#ifdef _WIN32
            LeaveCriticalSection(&bp->cs_print);
#else
            pthread_mutex_unlock(&bp->cs_print);
#endif
        }
    } while (true);
}


void brute(int argc, char** argv, ifstream* fp) {
    BruteParams bp(argc, argv, fp);

    time_t start, end;
    time(&start);

#ifdef _WIN32
    cout << SYS_MSG << "Launching windows threads..." << endl;
    HANDLE threads[bp.thread_count];
    for (int i = 0; i < bp.thread_count; ++i) {
        auto thread = (HANDLE) _beginthreadex(nullptr, 0, &brute_worker, (void*) &bp, 0, nullptr);
        threads[i] = thread;
    }

    WaitForMultipleObjects(bp.thread_count, threads, true, INFINITE);
#else
    cout << SYS_MSG << "Launching POSIX threads..." << endl;
    pthread_t threads[bp.thread_count];
    pthread_attr_t attrs[bp.thread_count];

    for (int i = 0; i < bp.thread_count; ++i) {
        pthread_attr_init(&attrs[i]);
        pthread_create(&threads[i], &attrs[i], brute_worker, (void *) &bp);
    }

    for (int i = 0; i < bp.thread_count; ++i) {
        pthread_join(threads[i], nullptr);
    }
#endif
    bp.print_stat(false);

    time(&end);
    double elapsed = difftime(end, start);
    cout << "Elapsed time: " << elapsed / 60 << " minutes" << endl;

    bp.write_skipped();
}