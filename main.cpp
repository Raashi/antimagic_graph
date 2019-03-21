#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iterator>

#include "perms.h"
#include "graph.h"

using namespace std;


unsigned long handle_first_line(char* line) {
    int index = 0;
    string g6(line);
    while ((int(g6[index])) >= 63)
        index++;
    return (unsigned long) index;
}

void write_unchecked(vector<string> unchecked) {
    ofstream output_file("unchecked.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(unchecked.begin(), unchecked.end(), output_iterator);
}

void brute(FILE* fp) {
    char* line                = nullptr;
    size_t len                = 0;
    bool first                = true;
    unsigned long true_length = 0;
    int checked_count         = 0;
    int non_antimagic         = 0;
    int skipped               = 0;
    // write here those graphs, that were checking for time > MAX_ANTIMAGIC_CALCULATION_TIME
    vector<string> unchecked = vector<string>();

    while ((getline(&line, &len, fp)) != -1) {
        if (first) {
            true_length = handle_first_line(line);
            first = false;
        }
        string g6(line, true_length);
        Graph g(g6);
        int antimagic = g.is_antimagic();

        checked_count++;
        if (antimagic == NON_ANTIMAGIC)
            non_antimagic++;
        else if (antimagic == TIME_OVERFLOW) {
            unchecked.push_back(g6);
            skipped++;
        }

        printf("\rChecked: %i Non-antimagic: %i Unchecked: %i\n", checked_count, non_antimagic, skipped);
        fflush(stdout);
    }

    if (line)
        free(line);

    if (!unchecked.empty())
        write_unchecked(unchecked);
}


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