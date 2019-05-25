#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <atomic>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "brute.h"
#include "utils.h"
#include "threads.h"


void AntimagicBruteParams::print_stat_inline() {
    if (checked % ITERATIONS_PRINT_INTERVAL == 0) {
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

uint AntimagicBruteParams::worker(string line) {
    Graph g(line);

    bool is_connected = g.is_connected();
    is_connected ? connected++ : not_connected++;

    Graph::AntimagicResult result = g.is_antimagic();
    checked++;

    if (result.antimagic) {
        antimagic++;
        is_connected ? connected_antimagic++ : not_connected_antimagic++;
    } else {
        not_antimagic++;
        is_connected ? connected_not_antimagic++ : not_connected_not_antimagic++;
        if (!result.optimized) {
            not_antimagic_not_optimized++;
            mutex_vec.lock();
            vec_not_optimized.push_back(line);
            mutex_vec.unlock();
        }
    }

    print_stat_inline();

    return WORKER_RETURN_OKAY;
}

void AntimagicBruteParams::finalize() {
    print_stat(false);
    printf("Connected all: %ld\n"
           "Connected and antimagic: %ld\n"
           "Connected and not antimagic: %ld\n",
           (long) connected,
           (long) connected_antimagic,
           (long) connected_not_antimagic);
    printf("Not connected: %ld\n"
           "Not connected and antimagic: %ld\n"
           "Not connected and not antimagic: %ld\n",
           (long) not_connected,
           (long) not_connected_antimagic,
           (long) not_connected_not_antimagic);
    printf("Not antimagic and not optimized: %ld\n", (long) not_antimagic_not_optimized);
    if (!vec_not_optimized.empty())
        write_to_file("not_antimagic_not_optimized.txt", vec_not_optimized);
}
