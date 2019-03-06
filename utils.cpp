#include <iostream>
#include <vector>
#include <cmath>

#include "utils.h"


void graph_free(Graph& graph) {
    for (int i = 0; i < graph.n; ++i)
        delete [] graph.matrix[i];
    delete [] graph.matrix;
}


void print_perm(const int* perm, const int n) {
    for (int i = 0; i < n; ++i)
        printf("%i ", perm[i]);
    printf("\n");
}

void print_graph(Graph& graph) {
    for (int i = 0; i < graph.n; ++i) {
        for (int j = 0; j < graph.n; ++j)
            std::printf("%i ", graph.matrix[i][j]);
        std::printf("\n");
    }
}