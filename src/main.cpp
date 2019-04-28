#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <exception>
#include <iterator>

#include "perms.h"
#include "graph.h"
#include "threads.h"
#include "brute.h"
#include "trees.h"

using namespace std;

const int BRUTE = 0;
const int G6 = 1;
const int G6_TO = 2;
const int G6_FROM = 3;
const int TREE = 4;
const int BRUTE_TREE = 5;

map<string, int> opts = { // NOLINT(cert-err58-cpp)
        {"brute", BRUTE},
        {"g6", G6},
        {"to", G6_TO},
        {"from", G6_FROM},
        {"tree", TREE},
        {"btree", BRUTE_TREE}
};

void _brute(int argc, char **argv);
void _brute_tree(int argc, char** argv);
void _test_tree(int, char** argv);
void _g6(int argc, char **argv);
void _g6_to();
void _g6_from(int argc, char **argv);


int main(int argc, char **argv) {
    if (argc < 2)
        throw runtime_error("Wrong arguments");

    string op = argv[1];
    switch (opts[op]) {
        case      BRUTE: _brute(argc, argv); break;
        case         G6: _g6(argc, argv); break;
        case       TREE: _test_tree(argc, argv); break;
        case BRUTE_TREE: _brute_tree(argc, argv); break;
        default        : break;
    }

    return 0;
}


void _brute(int argc, char **argv) {
    if (argc < 3)
        throw runtime_error("Wrong arguments");

    ifstream file(argv[2]);

    ThreadPull tp{argc, argv, &file};
    AntimagicBruteParams abp;
    tp.run(worker_antimagic, (void*) &abp, worker_antimagic_finalize);
    file.close();
}


void _brute_tree(int argc, char** argv) {
    if (argc < 3)
        throw runtime_error("Wrong arguments");

    ifstream file(argv[2]);

    ThreadPull tp{argc, argv, &file};
    TreesBruteParams tbp;
    tp.run(worker_trees, (void*) &tbp, worker_trees_finalize);
    file.close();
}


void _test_tree(int, char** argv) {
    string g6 = argv[2];
    Graph g(g6);

    numeration_t numer = numerate(&g);
    phi_t phi = numer.first;

    for (auto it : phi) {
        Edge e = it.first;
        cout << e.to_string() << " -> " << it.second << endl;
    }

    if (test_numeration(&g, phi))
        cout << "correct" << endl;
    else
        cout << "non-correct" << endl;
}


void _g6(int argc, char **argv) {
    if (argc == 2)
        throw runtime_error("Wrong arguments");

    string op = argv[2];
    switch (opts[op]) {
        case      G6_TO: _g6_to();
        case    G6_FROM: _g6_from(argc, argv);
        default        : break;
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


void _g6_from(int, char**) {
    string graph6;
    cout << "Enter graph in graph6 format: ";
    cin >> graph6;
    Graph(graph6).display();
}