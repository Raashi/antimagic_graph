#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>

#include "utils.h"

using namespace std;


typedef int Vertex;
typedef vector<Vertex> Vertices;
typedef vector<Vertices> VecVertices;


const uint ANTIMAGIC_YES = 1;
const uint ANTIMAGIC_NO = 0;
const uint ANTIMAGIC_SKIPPED = 2;


struct Edge {
    int a, b;

    Edge(int a, int b);
    string to_string();

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

    uint is_antimagic(bool skip, double time_overflow);
};


#endif //ANTIMAGIC_GRAPH_H
