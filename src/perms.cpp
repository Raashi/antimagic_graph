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
    free = perm_t(n, false);
    for (int i = 0; i < n; ++i) {
        perm.push_back(i);
    }
    start = true;

    this->randomize = randomize;
    this->rand_count = rand_count;
    rand_i = 0;
    g = mt19937_64(random_device{}());
}
#pragma clang diagnostic pop


bool PermGen::next() {
    if (randomize && rand_i < rand_count) {
        rand_i++;
        iota(begin(perm), end(perm), 0);
        shuffle(begin(perm), end(perm), g);
        return true;
    }

    if (start) {
        start = false;
        iota(begin(perm), end(perm), 0);
        return true;
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
        return true;
    }
    return false;
}
