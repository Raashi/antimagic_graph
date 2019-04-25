#ifndef ANTIMAGIC_BRUTE_H
#define ANTIMAGIC_BRUTE_H

#include <fstream>
#include <atomic>

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


uint worker_antimagic(void*, string);
void worker_antimagic_finalize(void*);

#endif //ANTIMAGIC_BRUTE_H
