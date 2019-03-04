#include <iostream>
#include <string>

#include "utils.h"
#include "perms.h"


int main() {
    /*
    std::cout << "Enter number of permutations: ";
    int n;
    std::cin >> n;

    PermGen gen{};
    int* perm = init_gen(gen, n);
    while (perm != nullptr) {
        print_perm(perm, n);
        perm = next(gen);
    }
     */
    std::cout << "Enter graph6: ";
    std::string graph6;
    std::cin >> graph6;
    int n = graph6_to_n(graph6);
    int** matrix = graph6_to_matrix(graph6);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            std::cout << matrix[i][j] << ' ';
        std::cout << std::endl;
    }

    return 0;
}