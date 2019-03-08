#include <iostream>
#include <string>

#include "perms.h"
#include "graph.h"


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
    // DQc

    // std::cout << "Enter graph6: ";
    std::string graph6 = "DQc";
    // std::cin >> graph6;

    Graph graph(graph6);
    graph.display();

    std::cout << std::endl;
    std::cout << graph.is_antimagic();

    return 0;
}