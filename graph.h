#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>


struct Edge {
    int a, b;

    Edge(int a, int b);

    bool operator < (const Edge& a) const;
};
typedef std::vector<Edge> Edges;


struct Graph {
    int n;
    int** matrix;

    explicit Graph(std::string& graph6);
    Graph(int n, Edges edges);
    ~Graph();

    Edges get_edges();
    void display();
    std::string to_graph6();

    bool is_antimagic();
};


#endif //ANTIMAGIC_GRAPH_H
