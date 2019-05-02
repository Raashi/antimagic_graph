#ifndef ANTIMAGIC_BRUTE_H
#define ANTIMAGIC_BRUTE_H

#include <fstream>
#include <atomic>
#include <vector>
#include <string>

#include "threads.h"
#include "utils.h"


struct AntimagicBruteParams {
    atomic_int checked{0};
    atomic_int antimagic{0};
    atomic_int non_antimagic{0};

    atomic_int connected{0};
    atomic_int connected_antimagic{0};
    atomic_int connected_non_antimagic{0};

    atomic_int not_connected{0};
    atomic_int not_connected_antimagic{0};
    atomic_int not_connected_non_antimagic{0};

    Mutex mutex_print;

    bool write_not_antimagic;
    vector<string> vec_not_antimagic;
    Mutex mutex_vec;

    bool check_only_not_connected;

    void print_stat(bool same_line);
    void print_stat_inline();

    explicit AntimagicBruteParams(bool write_not_antimagic, bool check_only_not_connected) {
        this->write_not_antimagic = write_not_antimagic;
        this->check_only_not_connected = check_only_not_connected;
    }
};


uint worker_antimagic(void*, string);
void worker_antimagic_finalize(void*);

#endif //ANTIMAGIC_BRUTE_H
