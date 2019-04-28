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
#include "trees.h"
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


uint worker_trees(void* arg, string line) {
    auto * wa = (WorkerArg*) arg;
    auto * bp = (TreesBruteParams*) wa->bp;

    Graph g(line);
    numeration_t numer = numerate(&g);
    phi_t phi = numer.first;

    bp->checked++;

    bool correct = test_numeration(&g, phi);
    numer.second ? bp->bis++ : bp->single++;

    if (correct)
        numer.second ? bp->bis_correct++ : bp->single_correct++;
    else {
        bp->non_antimagic++;
        bp->mutex_non_correct.lock();
        bp->non_correct.push_back(line);
        bp->mutex_non_correct.unlock();
    }

    if (bp->checked % 10 == 0) {
        bp->mutex_print.lock();
        printf("\rChecked: %i", (int) bp->checked);
        fflush(stdout);
        bp->mutex_print.unlock();
    }

    return WORKER_RETURN_OKAY;
}


void worker_trees_finalize(void* arg) {
    auto * wa = (WorkerArg*) arg;
    auto * bp = (TreesBruteParams*) wa->bp;

    cout << "\rChecked: " << bp->checked << endl << "-----------------------" << endl;
    cout << "Correct: " << (bp->checked - bp->non_antimagic) << " / " << bp->checked << endl;
    cout << "Correct single: " << bp->single_correct << " / " << bp->single << endl;
    cout << "Correct bi: " << bp->bis_correct << " / " << bp->bis << endl;

    if (!bp->non_correct.empty())
        write_to_file("noncorrect.txt", bp->non_correct);
}
