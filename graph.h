#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>


typedef int Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<Vertices> VecVertices;


const double MAX_ANTIMAGIC_CALCULATION_TIME = 1;

const int TIME_OVERFLOW = -1;
const int NON_ANTIMAGIC = 0;
const int ANTIMAGIC = 1;


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
    Vertices get_isolated();
    VecVertices get_adj_list();

    void display();

    std::string to_graph6();

    int is_antimagic();
};


#endif //ANTIMAGIC_GRAPH_H
