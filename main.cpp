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
        std::cout << "Enter graph6: " << std::endl;
        std::string g6;
        std::cin >> g6;

        Graph g(g6);
        std::cout << (g.is_antimagic() ? "True" : "False") << std::endl;

    } else if (op == "g6") {
        if (argc == 2)
            throw 1;

        std::string op_g6 = argv[2];
        if (op_g6 == "to") {
            int n, m;
            std::cout << "Enter number of vertices (n) and number of edges (m):\n";
            std::cin >> n >> m;
            Edges edges;
            std::cout << "Enter m edges:\n";
            for (int i = 0; i < m; ++i) {
                int a, b;
                std::cin >> a >> b;
                edges.emplace_back(a - 1, b - 1);
            }
            std::cout << Graph(n, edges).to_graph6() << std::endl;

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