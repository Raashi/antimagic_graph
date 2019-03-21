#include <iostream>
#include <string>

#include "perms.h"
#include "graph.h"
#include "brute.h"

using namespace std;


int main(int argc, char **argv) {
    if (argc < 2)
        throw -1;

    string op = argv[1];
    if (op == "brute") {
        if (argc < 3)
            throw 1;

        FILE* fp = fopen(argv[2], "r");
        if (fp == nullptr)
            throw 2;
        brute(fp);
        fclose(fp);
    } else if (op == "g6") {
        if (argc == 2)
            throw 1;

        string op_g6 = argv[2];
        if (op_g6 == "to") {
            int n, m;
            cout << "Enter number of vertices (n) and number of edges (m):\n";
            cin >> n >> m;
            Edges edges;
            cout << "Enter m edges:\n";
            for (int i = 0; i < m; ++i) {
                int a, b;
                cin >> a >> b;
                edges.emplace_back(a - 1, b - 1);
            }
            cout << Graph(n, edges).to_graph6() << endl;

        } else if (op_g6 == "from") {
            string graph6;
            cout << "Enter graph in graph6 format: ";
            cin >> graph6;
            Graph(graph6).display();
        } else
            throw 1;
    } else {
        throw 1;
    }

    return 0;
}