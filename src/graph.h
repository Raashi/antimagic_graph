#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>

using namespace std;


typedef int Vertex;
typedef vector<Vertex> Vertices;
typedef vector<Vertices> VecVertices;


const double MAX_ANTIMAGIC_CALCULATION_TIME = 1;

const int TIME_OVERFLOW = -1;
const int NON_ANTIMAGIC = 0;
const int ANTIMAGIC = 1;


struct Edge {
    int a, b;

    Edge(int a, int b);

    bool operator < (const Edge& a) const;
};
typedef vector<Edge> Edges;


struct Graph {
    int n;
    int** matrix;

    explicit Graph(string& graph6);
    Graph(int n, Edges edges);
    ~Graph();

    Edges get_edges();
    Vertices get_isolated();
    VecVertices get_adj_list();

    void display();

    string to_graph6();

    int is_antimagic(bool skip, double time_overflow);
};


#endif //ANTIMAGIC_GRAPH_H
