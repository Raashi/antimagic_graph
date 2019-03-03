#include <iostream>
#include <vector>

#include "utils.h"


void print_perm(const int* perm, const int n) {
    for (int i = 0; i < n; ++i)
        printf("%i ", perm[i]);
    printf("\n");
}