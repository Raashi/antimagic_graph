#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>

#include "perms.h"

using namespace std;


PermGen::PermGen(int n, bool randomize, int rand_count) {
    this->n = n;
    this->depth = n - 1;
    this->free = new int[n];
    this->perm = new int[n];
    for (int i = 0; i < n; ++i) {
        this->free[i] = false;
        this->perm[i] = i;
    }
    this->start = true;

    this->randomize = randomize;
    this->rand_count = rand_count;
    this->rand_i = 0;
    for (int i = 0; i < this->n; ++i)
        this->perm_rand.push_back(i);
    this->perm_rand_arr = new int[n];
}


int* PermGen::next() {
    if (this->start) {
        this->start = false;
        return this->perm;
    }

    if (this->randomize && this->rand_i < this->rand_count) {
        this->rand_i++;
        for (int i = 0; i < this->n; ++i)
            this->perm_rand[i] = i;
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(this->perm_rand.begin(), this->perm_rand.end(), mt19937(seed)); // default_random_engine(seed));
        for (int i = 0; i < this->n; ++i)
            perm_rand_arr[i] = perm_rand[i];
        return perm_rand_arr;
    }

    while (0 <= this->depth && this->depth < this->n) {
        if (this->perm[this->depth] >= 0)
            this->free[this->perm[this->depth]] = true;
        this->perm[this->depth]++;

        bool go_up = false;
        while (this->perm[this->depth] < this->n) {
            if (this->free[this->perm[this->depth]]) {
                this->free[this->perm[this->depth]] = false;
                this->depth++;
                go_up = true;
                break;
            }
            this->perm[this->depth]++;
        }
        if (!go_up) {
            this->perm[this->depth] = -1;
            this->depth--;
        }
    }
    if (this->depth == this->n) {
        this->depth--;
        return this->perm;
    } else
        return nullptr;
}

PermGen::~PermGen() {
    delete [] this->free;
    delete [] this->perm;
    delete [] this->perm_rand_arr;
}

void PermGen::display_last() {
    for (int i = 0; i < this->n; ++i)
        printf("%i, ", this->perm[i]);
}
