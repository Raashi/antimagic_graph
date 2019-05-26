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


void AntimagicBruteParams::worker(string line) {
    Graph g(line);

    bool is_connected = g.is_connected();
    is_connected ? connected++ : not_connected++;

    Graph::AntimagicResult result = g.is_antimagic();
    checked++;

    if (result.antimagic) {
        antimagic++;
        is_connected ? connected_antimagic++ : not_connected_antimagic++;
        if (result.randomized) {
            antimagic_randomized++;
            antimagic_randomized_iterations += result.iterations;
        }
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
}

void AntimagicBruteParams::finalize() {
    cout << SEPARATOR << endl;
    printf("All graph checked count : %ld\n", (long) checked);
    cout << SEPARATOR << endl;
    printf("Antimagic     : %ld\n"
           "Non-antimagic : %ld\n",
           (long) antimagic,
           (long) not_antimagic);
    cout << SEPARATOR << endl;
    printf("Antimagic randomized   : %ld\n"
           "Mean random iterations : %.2lf\n",
           (long) antimagic_randomized,
           (double) antimagic_randomized_iterations / antimagic_randomized);
    cout << SEPARATOR << endl;
    printf("Connected:\n"
           "   all           : %ld\n"
           "   antimagic     : %ld\n"
           "   not antimagic : %ld\n",
           (long) connected,
           (long) connected_antimagic,
           (long) connected_not_antimagic);
    cout << SEPARATOR << endl;
    printf("Not connected:\n"
           "   all           : %ld\n"
           "   antimagic     : %ld\n"
           "   not antimagic : %ld\n",
           (long) not_connected,
           (long) not_connected_antimagic,
           (long) not_connected_not_antimagic);
    cout << SEPARATOR << endl;
    printf("Not antimagic and not optimized: %ld\n", (long) not_antimagic_not_optimized);
    if (!vec_not_optimized.empty())
        write_to_file("not_antimagic_not_optimized.txt", vec_not_optimized);
}
