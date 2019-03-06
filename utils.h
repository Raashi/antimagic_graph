#ifndef ANTIMAGIC_UTILS_H
#define ANTIMAGIC_UTILS_H

#include <string>
#include <vector>

struct Edge {
    int a, b;
};

typedef std::vector<Edge> Edges;

struct Graph {
    int n;
    int** matrix;

    std::vector<Edge> get_edges() {
        std::vector<Edge> edges = std::vector<Edge>();
        for (int i = 0; i < this->n; ++i)
            for (int j = 0; j < i; ++j)
                if (this->matrix[i][j])
                    edges.push_back(Edge{i, j});
        return edges;
    }
};

void graph_free(Graph& graph);

void print_perm(const int* perm, int n);
void print_graph(Graph& graph);

#endif //ANTIMAGIC_UTILS_H
