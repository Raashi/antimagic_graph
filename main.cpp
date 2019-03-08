#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include "perms.h"
#include "graph.h"


unsigned long handle_first_line(char* line) {
    int index = 0;
    std::string g6(line);
    while ((int(g6[index])) >= 63)
        index++;
    return (unsigned long) index;
}


void brute(FILE* fp) {
    char* line = nullptr;
    size_t len = 0;
    std::vector<int> vec = std::vector<int>();
    bool first = true;
    unsigned long true_length = 0;
    int checked_count = 0;
    int non_antimagic = 0;
    while ((getline(&line, &len, fp)) != -1) {
        if (first) {
            true_length = handle_first_line(line);
            first = false;
        }
        std::string g6(line, true_length);
        Graph g(g6);
        bool antimagic = g.is_antimagic();

        checked_count++;
        if (!antimagic)
            non_antimagic++;

        printf("\r%i %i", checked_count, non_antimagic);
        fflush(stdout);
    }
    fclose(fp);
    if (line)
        free(line);
}


int main(int argc, char **argv) {
    if (argc < 2)
        throw -1;

    std::string op = argv[1];
    if (op == "brute") {
        if (argc < 3)
            throw 1;

        FILE* fp = fopen(argv[2], "r");
        if (fp == nullptr)
            throw 2;
        brute(fp);
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