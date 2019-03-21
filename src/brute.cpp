#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>

#include "graph.h"
#include "brute.h"

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

        printf("\rChecked: %i Non-antimagic: %i Unchecked: %i", checked_count, non_antimagic, skipped);
        fflush(stdout);
    }
    cout << endl;

    if (line)
        free(line);

    if (!unchecked.empty())
        write_unchecked(unchecked);
}