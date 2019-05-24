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


uint worker_antimagic(void* arg, string line) {
    auto * wa = (WorkerArg*) arg;
    auto * bp = (AntimagicBruteParams*) wa->bp;

    Graph g(line);
    // bp->print_stat_inline(line);

    bool connected = g.is_connected();
    connected ? bp->connected++ : bp->not_connected++;

    if (bp->check_only_not_connected && !connected)
        return WORKER_RETURN_OKAY;

    bool antimagic = g.is_antimagic();
    bp->checked++;

    if (antimagic) {
        bp->antimagic++;
        connected ? bp->connected_antimagic++ : bp->not_connected_antimagic++;
    }
    else {
        bp->non_antimagic++;
        connected ? bp->connected_non_antimagic++ : bp->not_connected_non_antimagic++;

        if (bp->write_not_antimagic) {
            bp->mutex_vec.lock();
            bp->vec_not_antimagic.push_back(line);
            bp->mutex_vec.unlock();
        }
    }

    bp->print_stat_inline();

    return WORKER_RETURN_OKAY;
}

void worker_antimagic_finalize(void* arg) {
    auto * wa = (WorkerArg*) arg;
    auto * bp = (AntimagicBruteParams*) wa->bp;
    bp->print_stat(false);
    printf("Connected all: %i\n"
           "Connected and antimagic: %i\n"
           "Connected and not antimagic: %i\n",
           (int) bp->connected,
           (int) bp->connected_antimagic,
           (int) bp->connected_non_antimagic);
    printf("Not connected: %i\n"
           "Not connected and antimagic: %i\n"
           "Not connected and not antimagic: %i\n",
           (int) bp->not_connected,
           (int) bp->not_connected_antimagic,
           (int) bp->not_connected_non_antimagic);
    if (bp->write_not_antimagic)
        write_to_file("not_antimagic.txt", bp->vec_not_antimagic);
}

void AntimagicBruteParams::print_stat_inline() {
    if (this->checked % 10000 == 0) {
        this->mutex_print.lock();
        this->print_stat(true);
        fflush(stdout);
        this->mutex_print.unlock();
    }
}

void AntimagicBruteParams::print_stat(bool same_line) {
    printf("\rChecked: %i Non-antimagic: %i %s",
            (int) this->checked,
            (int) this->non_antimagic,
            same_line ? "" : "\n");
}
