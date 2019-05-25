#ifndef ANTIMAGIC_PERMS_H
#define ANTIMAGIC_PERMS_H

#include <vector>
#include <random>
#include "utils.h"

using namespace std;


typedef vector<int> perm_t;

struct PermGen {
    int n;
    int depth;
    perm_t free;
    perm_t perm;
    bool start;

    mt19937_64 g;

    bool randomize;
    uint rand_count;
    uint rand_i;

    explicit PermGen(int n, bool randomize, uint rand_count);

    bool next();
};

#endif //ANTIMAGIC_PERMS_H
