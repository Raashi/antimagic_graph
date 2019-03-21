#include <iostream>
#include <string>
#include <map>

#include "perms.h"
#include "graph.h"
#include "brute.h"

using namespace std;

const int BRUTE = 0;
const int G6 = 1;
const int G6_TO = 2;
const int G6_FROM = 3;
map<string, int> opts = { // NOLINT(cert-err58-cpp)
        {"brute", BRUTE},
        {"g6", G6},
        {"to", G6_TO},
        {"from", G6_FROM}
};

void _brute(int argc, char **argv);
void _g6(int argc, char **argv);
void _g6_to();
void _g6_from(int argc, char **argv);


int main(int argc, char **argv) {
    if (argc < 2)
        throw -1;

    string op = argv[1];
    switch (opts[op]) {
        case BRUTE: _brute(argc, argv);
        case G6: _g6(argc, argv);
        default: break;
    }

    return 0;
}

void _brute(int argc, char **argv) {
    if (argc < 3)
        throw 1;

    FILE* fp = fopen(argv[2], "r");
    if (fp == nullptr) throw 2;
    brute(fp);
    fclose(fp);
}

void _g6(int argc, char **argv) {
    if (argc == 2)
        throw 1;

    string op = argv[2];
    switch (opts[op]) {
        case G6_TO: _g6_to();
        case G6_FROM: _g6_from(argc, argv);
        default: break;
    }
}

void _g6_to() {
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
}

void _g6_from(int argc, char **argv) {
    string graph6;
    cout << "Enter graph in graph6 format: ";
    cin >> graph6;
    Graph(graph6).display();
}