#include <iostream>
#include <vector>

#include "utils.h"


void print_perm(const int* perm, const int n) {
    for (int i = 0; i < n; ++i)
        printf("%i ", perm[i]);
    printf("\n");
}

int graph6_to_n(std::string & graph6) {
    return int(graph6[0]) - 63;
}

int** graph6_to_matrix(std::string & graph6) {
    // initialization
    int n = graph6_to_n(graph6);
    int** matrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new int[n];
        // put zeros on main
        matrix[i][i] = 0;
    }
    // parsing
    int row = 0;
    int col = 1;
    for (int i = 1; i < graph6.size(); ++i) {
        auto isymbol = short(graph6[i]);
        for (int ibit = 0; ibit < 8; ibit++) {
            int bit = isymbol % 2;
            isymbol >>= 1;
            matrix[row][col] = bit;
            matrix[col][row] = bit;
            // update indices
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
        }
    }
    return matrix;
}

int** graph6_to_list(std::string & graph6) {
    return nullptr;
}