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
    int antimagic = g.is_antimagic(wa->tp->skip, wa->tp->skip_time);

    bp->checked++;
    if (antimagic == ANTIMAGIC_NO)
        bp->non_antimagic++;
    else if (antimagic == ANTIMAGIC_SKIPPED)
        bp->skipped++;

    bp->print_stat_inline();

    return antimagic == ANTIMAGIC_SKIPPED ? WORKER_RETURN_SKIPPED : WORKER_RETURN_OKAY;
}

void worker_antimagic_finalize(void* arg) {
    auto * wa = (WorkerArg*) arg;
    auto * bp = (AntimagicBruteParams*) wa->bp;
    bp->print_stat(false);
}

void AntimagicBruteParams::print_stat_inline() {
    if (this->checked % 10 == 0) {
        this->mutex_print.lock();
        this->print_stat(true);
        fflush(stdout);
        this->mutex_print.unlock();
    }
}

void AntimagicBruteParams::print_stat(bool same_line) {
    if (this->skipped > 0)
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
}
