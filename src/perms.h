#ifndef ANTIMAGIC_PERMS_H
#define ANTIMAGIC_PERMS_H

#include <vector>

using namespace std;


struct PermGen {
    int n;
    int depth;
    int* free;
    int* perm;
    bool start;

    bool randomize;
    int rand_count;
    int rand_i;
    vector<int> perm_rand;
    int* perm_rand_arr;

    explicit PermGen(int n, bool randomize, int rand_count);
    int* next();

    ~PermGen();

    void display_last();
};

#endif //ANTIMAGIC_PERMS_H
