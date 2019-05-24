#ifndef ANTIMAGIC_PERMS_H
#define ANTIMAGIC_PERMS_H

#include <vector>
#include <random>
#include "utils.h"

using namespace std;


struct PermGen {
    int n;
    int depth;
    int* free;
    int* perm;
    bool start;

    mt19937_64 g;

    bool randomize;
    uint rand_count;
    uint rand_i;
    vector<int> perm_rand;
    int* perm_rand_arr;

    explicit PermGen(int n, bool randomize, uint rand_count);
    int* next();

    ~PermGen();
};

#endif //ANTIMAGIC_PERMS_H
