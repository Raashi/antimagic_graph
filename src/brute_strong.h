#ifndef ANTIMAGIC_BRUTE_STRONG_H
#define ANTIMAGIC_BRUTE_STRONG_H

#include "brute.h"

struct StrongAntimagicBruteParams {
    AntimagicBruteParams* abp;
    int increment_max;

    StrongAntimagicBruteParams(AntimagicBruteParams*, int increment_max);

    void print_stat(bool same_line);
    void print_stat_inline();
};


uint worker_strong_antimagic(void*, string);
void worker_strong_antimagic_finalize(void*);

#endif //ANTIMAGIC_BRUTE_STRONG_H
