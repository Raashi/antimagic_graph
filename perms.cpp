#include <vector>

#include "perms.h"


PermGen::PermGen(int n) {
    this->n = n;
    this->depth = n - 1;
    this->free = new int[n];
    this->perm = new int[n];
    for (int i = 0; i < n; ++i) {
        this->free[i] = false;
        this->perm[i] = i;
    }
    this->start = true;
}


int* PermGen::next() {
    if (this->start) {
        this->start = false;
        return this->perm;
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
    } else {
        delete[] this->perm;
        delete[] this->free;
        return nullptr;
    }
}