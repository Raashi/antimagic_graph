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
    ulong thread_count;

    explicit BruteParams (int argc, char** argv, ifstream* fp) {
        InitializeCriticalSection(&this->cs);
        InitializeCriticalSection(&this->cs_skipped);
        InitializeCriticalSection(&this->cs_print);

        this->fp = fp;

        this->thread_count = get_arg(argc, argv, "-tc", THREAD_COUNT_DEFAULT);
        cout << SYS_MSG << "Launching " << thread_count << " threads" << endl;

        this->skip = has_arg(argc, argv, "-skip");
        this->skip_time = get_arg(argc, argv, "-skt", MAX_ANTIMAGIC_CALCULATION_TIME);
        this->vec_skipped = vector<string>();

        if (this->skip)
            cout << SYS_MSG << "Maximum time for calculating graph was set to "
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
            bp->print_stat(true);
            fflush(stdout);
            LeaveCriticalSection(&bp->cs_print);
        }
    } while (true);
}


void brute(int argc, char** argv, ifstream* fp) {
    BruteParams bp(argc, argv, fp);

    time_t start, end;
    time(&start);


    HANDLE threads[bp.thread_count];
    for (int i = 0; i < bp.thread_count; ++i) {
        auto thread = (HANDLE) _beginthreadex(nullptr, 0, &brute_worker, (void*) &bp, 0, nullptr);
        threads[i] = thread;
    }

    WaitForMultipleObjects(bp.thread_count, threads, true, INFINITE);

    bp.print_stat(false);
    time(&end);
    double elapsed = difftime(end, start);
    cout << "Elapsed time: " << elapsed / 60 << " minutes" << endl;

    bp.write_skipped();
}