#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>
#include <set>
#include <map>

#include "utils.h"

using namespace std;


typedef int Vertex;
typedef vector<Vertex> Vertices;
typedef vector<Vertices> VecVertices;

const uint INFINITE_DISTANCE = UINT_MAX;


struct Edge {
    int a, b;

    Edge(int a, int b);

    bool operator < (const Edge& a) const;
};
typedef vector<Edge> Edges;


typedef map<Edge, int> phi_t;


struct Graph {
    int n;
    int m;
    int** matrix;
    VecVertices adj;

    explicit Graph(string& graph6);
    Graph(int n, const Edges& edges);
    ~Graph();

    Edges get_edges();
    Vertices get_isolated();

    void init_adj_list();

    void display();

    string to_graph6();

    bool is_connected();
    uint is_antimagic(int increment = 0);

    vector<uint> get_all_distances(Vertex, set<Vertex>* = nullptr);
};


#endif //ANTIMAGIC_GRAPH_H
