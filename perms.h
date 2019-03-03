#ifndef ANTIMAGIC_PERMS_H
#define ANTIMAGIC_PERMS_H

#include <vector>

struct PermGen {
    int n;
    int depth;
    int* free;
    int* perm;
};

int* init_gen(PermGen& gen, int n);

int* next(PermGen& gen);

#endif //ANTIMAGIC_PERMS_H
