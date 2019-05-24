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


uint worker_antimagic(void *arg, string line) {
    auto *wa = (WorkerArg *) arg;
    auto *bp = (AntimagicBruteParams *) wa->bp;

    Graph g(line);

    bool connected = g.is_connected();
    connected ? bp->connected++ : bp->not_connected++;

    Graph::AntimagicResult result = g.is_antimagic();
    bp->checked++;

    if (result.antimagic) {
        bp->antimagic++;
        connected ? bp->connected_antimagic++ : bp->not_connected_antimagic++;
    } else {
        bp->not_antimagic++;
        connected ? bp->connected_not_antimagic++ : bp->not_connected_not_antimagic++;
        if (!result.optimized) {
            bp->not_antimagic_not_optimized++;
            bp->mutex_vec.lock();
            bp->vec_not_optimized.push_back(line);
            bp->mutex_vec.unlock();
        }
        bp->mutex_vec.lock();
        bp->vec_not_antimagic.push_back(line);
        bp->mutex_vec.unlock();
    }

    bp->print_stat_inline();

    return WORKER_RETURN_OKAY;
}

void worker_antimagic_finalize(void *arg) {
    auto *wa = (WorkerArg *) arg;
    auto *bp = (AntimagicBruteParams *) wa->bp;
    bp->print_stat(false);
    printf("Connected all: %ld\n"
           "Connected and antimagic: %ld\n"
           "Connected and not antimagic: %ld\n",
           (long) bp->connected,
           (long) bp->connected_antimagic,
           (long) bp->connected_not_antimagic);
    printf("Not connected: %i\n"
           "Not connected and antimagic: %ld\n"
           "Not connected and not antimagic: %ld\n",
           (long) bp->not_connected,
           (long) bp->not_connected_antimagic,
           (long) bp->not_connected_not_antimagic);
    printf("Not antimagic and not optimized: %ld\n", (long) bp->not_antimagic_not_optimized);
    if (!bp->vec_not_optimized.empty())
        write_to_file("not_antimagic_not_optimized.txt", bp->vec_not_optimized);
    if (!bp->vec_not_antimagic.empty())
        write_to_file("not_antimagic.txt", bp->vec_not_antimagic);
}

void AntimagicBruteParams::print_stat_inline() {
    if (checked % 10000 == 0) {
        mutex_print.lock();
        print_stat(true);
        fflush(stdout);
        mutex_print.unlock();
    }
}

void AntimagicBruteParams::print_stat(bool same_line) {
    printf("\rChecked: %ld Non-antimagic: %ld %s",
           (long) checked,
           (long) not_antimagic,
           same_line ? "" : "\n");
}
