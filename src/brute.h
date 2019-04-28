#ifndef ANTIMAGIC_BRUTE_H
#define ANTIMAGIC_BRUTE_H

#include <fstream>
#include <atomic>
#include <vector>

#include "threads.h"
#include "utils.h"


struct AntimagicBruteParams {
    atomic_int checked{0};
    atomic_int non_antimagic{0};
    atomic_int skipped{0};

    Mutex mutex_print;

    void print_stat(bool same_line);
    void print_stat_inline();
};


struct TreesBruteParams {
    atomic_int checked{0};
    atomic_int non_antimagic{0};
    atomic_int single{0};
    atomic_int single_correct{0};
    atomic_int bis{0};
    atomic_int bis_correct{0};

    Mutex mutex_print;
    Mutex mutex_non_correct;

    vector<string> non_correct;
};


uint worker_antimagic(void*, string);
void worker_antimagic_finalize(void*);

uint worker_trees(void*, string);
void worker_trees_finalize(void*);

#endif //ANTIMAGIC_BRUTE_H
