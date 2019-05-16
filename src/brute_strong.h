#ifndef ANTIMAGIC_BRUTE_STRONG_H
#define ANTIMAGIC_BRUTE_STRONG_H

#include "brute.h"

struct StrongAntimagicBruteParams {
    AntimagicBruteParams* abp;

    int increment_max;

    StrongAntimagicBruteParams (AntimagicBruteParams* abp, int increment_max) {
        this->abp = abp;
        this->increment_max = increment_max;
    }
};


uint worker_strong_antimagic(void*, string);
void worker_strong_antimagic_finalize(void*);

#endif //ANTIMAGIC_BRUTE_STRONG_H
