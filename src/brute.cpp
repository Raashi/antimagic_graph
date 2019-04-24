#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <atomic>
#include <windows.h>
#include <process.h>
#include <time.h>

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

    CRITICAL_SECTION cs{};
    CRITICAL_SECTION cs_skipped{};
    CRITICAL_SECTION cs_print{};

    ifstream* fp;

    explicit BruteParams (int argc, char** argv, ifstream* fp) {
        InitializeCriticalSection(&this->cs);
        InitializeCriticalSection(&this->cs_skipped);
        InitializeCriticalSection(&this->cs_print);

        this->fp = fp;

        this->skip = has_arg(argc, argv, "-skip");
        this->skip_time = get_arg(argc, argv, "-skt", MAX_ANTIMAGIC_CALCULATION_TIME);
        this->vec_skipped = vector<string>();
    };
};


void print_stat(BruteParams* bp, bool same_line) {
    if (bp->skip)
        printf("\rChecked: %i Non-antimagic: %i Unchecked: %i%s",
                (int) bp->checked,
                (int) bp->non_antimagic,
                (int) bp->skipped,
                same_line ? "" : "\n");
    else
        printf("\rChecked: %i Non-antimagic: %i%s",
                (int) bp->checked,
                (int) bp->non_antimagic,
                same_line ? "" : "\n");
}


void write_unchecked(vector<string> unchecked) {
    ofstream output_file("skipped.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(unchecked.begin(), unchecked.end(), output_iterator);
}


unsigned int __stdcall brute_worker(void * param) {
    auto * bp = (BruteParams*) param;
    string line;

    do {
        bool leave = false;
        EnterCriticalSection(&bp->cs);
        if (not (bool) getline(*(bp->fp), line))
            leave = true;
        LeaveCriticalSection(&bp->cs);

        if (leave) _endthreadex(0);

        Graph g(line);
        int antimagic = g.is_antimagic(bp->skip, bp->skip_time);

        bp->checked++;
        if (antimagic == NON_ANTIMAGIC)
            bp->non_antimagic++;
        else if (antimagic == TIME_OVERFLOW) {
            EnterCriticalSection(&bp->cs_skipped);
            bp->vec_skipped.push_back(line);
            LeaveCriticalSection(&bp->cs_skipped);
            bp->skipped++;
        }

        if (bp->checked % 10 == 0) {
            EnterCriticalSection(&bp->cs_print);
            print_stat(bp, true);
            fflush(stdout);
            LeaveCriticalSection(&bp->cs_print);
        }
    } while (true);
}


void brute(int argc, char** argv, ifstream* fp) {
    BruteParams bp(argc, argv, fp);

    time_t start, end;
    time(&start);

    DWORD thread_count = get_arg(argc, argv, "-tc", THREAD_COUNT_DEFAULT);
    cout << SYS_MSG << "Launching " << thread_count << " threads" << endl;
    HANDLE threads[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        auto thread = (HANDLE) _beginthreadex(nullptr, 0, &brute_worker, (void*) &bp, 0, nullptr);
        threads[i] = thread;
    }

    WaitForMultipleObjects(thread_count, threads, true, INFINITE);

    print_stat(&bp, false);
    time(&end);
    double elapsed = difftime(end, start);
    cout << "Elapsed time: " << elapsed / 60 << " minutes" << endl;

    if (!bp.vec_skipped.empty())
        write_unchecked(bp.vec_skipped);
}