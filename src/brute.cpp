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
#include "threads.h"

using namespace std;


struct BruteParams {
    atomic_int checked{0};
    atomic_int non_antimagic{0};

    bool skip;
    double skip_time;
    atomic_int skipped{0};
    vector<string> vec_skipped;

    Mutex mutex;
    Mutex mutex_skipped;
    Mutex mutex_print;

    ifstream* fp;
    ulong thread_count;

    BruteParams (int argc, char** argv, ifstream* fp) {
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
        bp->mutex.lock();
        if (not (bool) getline(*(bp->fp), line))
            leave = true;
        bp->mutex.unlock();
        if (leave)
#ifdef _WIN32
            _endthreadex(0);
#else
            pthread_exit(nullptr);
#endif

        Graph g(line);
        int antimagic = g.is_antimagic(bp->skip, bp->skip_time);

        bp->checked++;
        if (antimagic == NON_ANTIMAGIC)
            bp->non_antimagic++;
        else if (antimagic == TIME_OVERFLOW) {
            bp->mutex_skipped.lock();
            bp->vec_skipped.push_back(line);
            bp->mutex_skipped.unlock();
            bp->skipped++;
        }

        if (bp->checked % 10 == 0) {
            bp->mutex_print.lock();
            bp->print_stat(true);
            fflush(stdout);
            bp->mutex_print.unlock();
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