#ifndef ANTIMAGIC_GRAPH_H
#define ANTIMAGIC_GRAPH_H

#include <string>
#include <set>
#include <map>

#include "utils.h"

using namespace std;


typedef uint Vertex;
typedef vector<Vertex> Vertices;
typedef vector<Vertices> VecVertices;

const uint INFINITE_DISTANCE = UINT_MAX;
const uint RANDOM_PERMUTATIONS_COUNT = 200000u;


struct Edge {
    Vertex a, b;

    Edge(Vertex a, Vertex b);

    bool operator < (const Edge& a) const;
};
typedef vector<Edge> Edges;


typedef map<Edge, uint> phi_t;


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
