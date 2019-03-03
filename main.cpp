#include <iostream>
#include <vector>

#include "utils.h"
#include "perms.h"


int main() {
    std::cout << "Enter number of permutations: ";
    int n;
    std::cin >> n;

    PermGen gen{};
    int* perm = init_gen(gen, n);
    while (perm != nullptr) {
        print_perm(perm, n);
        perm = next(gen);
    }
    return 0;
}