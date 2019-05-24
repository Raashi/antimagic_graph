#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <functional>

#include "perms.h"
#include "utils.h"

using namespace std;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc32-c"
PermGen::PermGen(int n, bool randomize, uint rand_count) {
    this->n = n;
    depth = n - 1;
    free = new int[n];
    perm = new int[n];
    for (int i = 0; i < n; ++i) {
        free[i] = false;
        perm[i] = i;
    }
    start = true;

    this->randomize = randomize;
    this->rand_count = rand_count;
    rand_i = 0;
    for (int i = 0; i < n; ++i)
        perm_rand.push_back(i);
    perm_rand_arr = new int[n];
    g = mt19937_64(random_device{}());
}
#pragma clang diagnostic pop


int* PermGen::next() {
    if (start) {
        start = false;
        return perm;
    }

    if (randomize && rand_i < rand_count) {
        rand_i++;
        for (int i = 0; i < n; ++i)
            perm_rand[i] = i;
        shuffle(perm_rand.begin(), perm_rand.end(), g);
        for (int i = 0; i < n; ++i)
            perm_rand_arr[i] = perm_rand[i];
        return perm_rand_arr;
    }

    while (0 <= depth && depth < n) {
        if (perm[depth] >= 0)
            free[perm[depth]] = true;
        perm[depth]++;

        bool go_up = false;
        while (perm[depth] < n) {
            if (free[perm[depth]]) {
                free[perm[depth]] = false;
                depth++;
                go_up = true;
                break;
            }
            perm[depth]++;
        }
        if (!go_up) {
            perm[depth] = -1;
            depth--;
        }
    }
    if (depth == n) {
        depth--;
        return perm;
    } else
        return nullptr;
}

PermGen::~PermGen() {
    delete [] free;
    delete [] perm;
    delete [] perm_rand_arr;
}
