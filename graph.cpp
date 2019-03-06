#include <iostream>
#include <map>

#include "perms.h"
#include "graph.h"


Graph from_graph6(std::string & graph6) {
    // initialization
    int n = int(graph6[0]) - 63;
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
        auto isymbol = short(graph6[i]) - 63;
        for (int ibit = 5; ibit >= 0; --ibit) {
            int bit = (isymbol >> ibit) & 1;
            matrix[row][col] = bit;
            matrix[col][row] = bit;
            // update indices
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
            if (col == n)
                return Graph{n, matrix};
        }
    }
    raise(6);
}

bool is_antimagic(Graph& graph) {
    Edges edges = graph.get_edges();
    for (Edge e: edges)
        std::cout << e.a << ' ' << e.b << std::endl;

    PermGen gen = PermGen(int(edges.size()));
    int* perm = gen.next();
    while (perm != nullptr) {
        print_perm(perm, graph.n);
        // TODO: complete phi: Edge -> int
        // std::map<Edge, int> phi = std::map<Edge, int>();
        // for (int i = 0; i < edges.size(); ++i)
            // phi[edges[i]] = perm[i];

        int* sums = new int[graph.n];
        for (int i = 0; i < graph.n; ++i) {
            int sum = 0;
        }

        perm = gen.next();
    }
    return false;
}
