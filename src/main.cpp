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
void _g6(int argc, char **argv);
void _g6_to();
void _g6_from(int argc, char **argv);

void test_tree(int, char** argv) {
    string g6 = argv[2];
    Graph g(g6);
    numeration_t numer = numerate(&g);
    map<Edge, int> phi = numer.first;

    for (auto it : phi) {
        Edge e = it.first;
        cout << e.to_string() << " -> " << it.second << endl;
    }

    if (test_numeration(&g, phi))
        cout << "correct" << endl;
    else
        cout << "non-correct" << endl;
}

void brute_tree(int, char** argv) {
    ifstream file(argv[2]);

    int count = 0;
    int all = 0;
    int single_correct = 0;
    int singles = 0;
    int bi_correct = 0;
    int bis = 0;

    vector<string> noncorrect;

    string line;
    while (true) {
        if (not (bool) getline(file, line))
            break;

        Graph g(line);
        numeration_t numer = numerate(&g);
        map<Edge, int> phi = numer.first;

        all++;

        bool correct = test_numeration(&g, phi);
        if (numer.second)
            bis++;
        else
            singles++;
        if (correct) {
            count++;
            if (numer.second)
                bi_correct++;
            else
                single_correct++;
        } else
            noncorrect.push_back(line);
    }
    file.close();
    cout << "Correct: " << count << " / " << all << endl;
    cout << "Correct single: " << single_correct << " / " << singles << endl;
    cout << "Correct bi: " << bi_correct << " / " << bis << endl;

    ofstream output_file("noncorrect.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(noncorrect.begin(), noncorrect.end(), output_iterator);
}


int main(int argc, char **argv) {
    if (argc < 2)
        throw runtime_error("Wrong arguments");

    string op = argv[1];
    switch (opts[op]) {
        case      BRUTE: _brute(argc, argv);
        case         G6: _g6(argc, argv);
        case       TREE: test_tree(argc, argv); break;
        case BRUTE_TREE: brute_tree(argc, argv);
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

void _g6_from(int argc, char **argv) {
    string graph6;
    cout << "Enter graph in graph6 format: ";
    cin >> graph6;
    Graph(graph6).display();
}