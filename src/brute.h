#ifndef ANTIMAGIC_BRUTE_H
#define ANTIMAGIC_BRUTE_H

#include <fstream>
#include <atomic>
#include <vector>
#include <string>

#include "threads.h"
#include "utils.h"


struct AntimagicBruteParams {
    atomic_long checked{0};
    atomic_long antimagic{0};
    atomic_long not_antimagic{0};
    atomic_long not_antimagic_not_optimized{0};

    atomic_long connected{0};
    atomic_long connected_antimagic{0};
    atomic_long connected_not_antimagic{0};

    atomic_long not_connected{0};
    atomic_long not_connected_antimagic{0};
    atomic_long not_connected_not_antimagic{0};

    Mutex mutex_print;

    vector<string> vec_not_optimized;
    vector<string> vec_not_antimagic;
    Mutex mutex_vec;

    void print_stat(bool same_line);
    void print_stat_inline();
};


uint worker_antimagic(void*, string);
void worker_antimagic_finalize(void*);

#endif //ANTIMAGIC_BRUTE_H
