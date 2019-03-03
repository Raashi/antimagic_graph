#include <vector>

#include "perms.h"


int* init_gen(PermGen& gen, const int n) {
    gen.n = n;
    gen.depth = n - 1;
    gen.free = new int[n];
    gen.perm = new int[n];
    for (int i = 0; i < n; ++i) {
        gen.free[i] = false;
        gen.perm[i] = i;
    }
    return gen.perm;
}

int* next(PermGen& gen) {
    while (0 <= gen.depth && gen.depth < gen.n) {
        if (gen.perm[gen.depth] >= 0)
            gen.free[gen.perm[gen.depth]] = true;
        gen.perm[gen.depth]++;

        bool go_up = false;
        while (gen.perm[gen.depth] < gen.n) {
            if (gen.free[gen.perm[gen.depth]]) {
                gen.free[gen.perm[gen.depth]] = false;
                gen.depth++;
                go_up = true;
                break;
            }
            gen.perm[gen.depth]++;
        }
        if (!go_up) {
            gen.perm[gen.depth] = -1;
            gen.depth--;
        }
    }
    if (gen.depth == gen.n) {
        gen.depth--;
        return gen.perm;
    } else {
        delete[] gen.perm;
        delete[] gen.free;
        return nullptr;
    }
}