#include <iostream>
#include <string>
#include <stdexcept>

#include "perms.h"
#include "graph.h"


int main(int argc, char **argv) {
    if (argc < 2)
        throw -1;

    std::string op = argv[1];
    if (op == "brute") {
        //
    } else if (op == "g6") {
        if (argc == 2)
            throw 1;

        std::string op_g6 = argv[2];
        if (op_g6 == "to") {
            int n;
            std::cout << "Enter number of edges: ";
            std::cin >> n;
            Edges edges;
            std::cout << "Enter n edges:\n";
            for (int i = 0; i < n; ++i) {
                int a, b;
                std::cin >> a >> b;
                edges.emplace_back(a, b);
            }

        } else if (op_g6 == "from") {
            std::string graph6;
            std::cout << "Enter graph in graph6 format: ";
            std::cin >> graph6;
            Graph(graph6).display();
        } else
            throw 1;
    } else {
        throw 1;
    }

    return 0;
}