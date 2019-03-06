#include <iostream>
#include <string>

#include "utils.h"
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

    std::cout << "Enter graph6: ";
    std::string graph6;
    std::cin >> graph6;

    Graph graph = from_graph6(graph6);
    print_graph(graph);
    std::cout << std::endl;
    std::cout << is_antimagic(graph);

    graph_free(graph);
    return 0;
}