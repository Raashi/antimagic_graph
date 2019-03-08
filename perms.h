#ifndef ANTIMAGIC_PERMS_H
#define ANTIMAGIC_PERMS_H

#include <vector>

struct PermGen {
    int n;
    int depth;
    int* free;
    int* perm;
    bool start;

    explicit PermGen(int n);
    int* next();

    ~PermGen();
};

#endif //ANTIMAGIC_PERMS_H
