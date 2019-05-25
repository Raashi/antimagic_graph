#include <iostream>
#include <map>
#include <time.h>
#include <queue>
#include <exception>
#include <algorithm>

#include <chrono>

#include "perms.h"
#include "graph.h"
#include "utils.h"

using namespace std;


Edge::Edge(const Vertex a, const Vertex b) {
    if (a < b) {
        this->a = a;
        this->b = b;
    } else {
        this->a = b;
        this->b = a;
    }
}

bool Edge::operator<(const Edge &right) const {
    if (b != right.b)
        return b < right.b;
    else
        return a < right.a;
}

Graph::Graph(string &graph6) {
    n = int(graph6[0]) - 63;
    m = 0;
    matrix = new int *[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new int[n];
        // put zeros on main
        matrix[i][i] = 0;
    }
    // parsing
    int row = 0;
    int col = 1;
    for (int i = 1; i < graph6.size(); ++i) {
        uint isymbol = uint(graph6[i]) - 63u;
        for (uint ibit = 5; ibit <= 5; --ibit) {
            uint bit = (isymbol >> ibit) & 1u;
            matrix[row][col] = bit;
            matrix[col][row] = bit;
            m += bit;
            // update indices
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
            if (col == n) {
                init_adj_list();
                return;
            }
        }
    }
    throw runtime_error("Error decoding graph6: " + graph6);
}

Graph::~Graph() {
    for (int i = 0; i < n; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

Edges Graph::get_edges() {
    Edges edges;

    vector<int> js(n, 0);
    while (edges.size() < m) {
        for (int i = n - 1; i >= 0; --i)
            for (int j = js[i]; j < i; ++j) {
                if (matrix[i][j]) {
                    edges.emplace_back(i, j);
                    js[i] += 1;
                    break;
                }
                js[i] += 1;
            }
    }
    return edges;
}

Vertices Graph::get_isolated() {
    Vertices vertices = Vertices();
    for (int i = 0; i < this->n; ++i) {
        bool isolated = true;
        for (int j = 0; j < this->n; ++j)
            if (this->matrix[i][j]) {
                isolated = false;
                break;
            }
        if (isolated)
            vertices.push_back(i);
    }
    return vertices;
}

void Graph::init_adj_list() {
    if (!adj.empty())
        return;
    adj = VecVertices((unsigned long) n, Vertices());
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (matrix[i][j])
                adj[i].push_back(j);
}

Graph::AntimagicResult Graph::is_antimagic(int increment) {
    // 1st optimization: isolated vertices count must be <= 1
    if (get_isolated().size() > 1)
        return AntimagicResult{false, true};
    // 2nd optimization: if there is K2 connected component in G
    init_adj_list();
    for (int i = 0; i < n; ++i)
        if (adj[i].size() == 1)
            if (adj[adj[i][0]].size() == 1 && adj[adj[i][0]][0] == i)
                return AntimagicResult{false, true};

    Edges edges = get_edges();
    PermGen gen(int(edges.size()), true, RANDOM_PERMUTATIONS_COUNT);
    phi_t phi = phi_t();
    vector<uint> f = vector<uint>(n, 0);

    while (gen.next()) {
        for (int i = 0; i < edges.size(); ++i)
            phi[edges[i]] = gen.perm.at(i) + 1 + increment;  // phi: E -> [1, 2, ..., edges_count]

        // calc sum of phi for every vertex
        for (int i = 0; i < n; ++i) {
            f[i] = 0;
            for (int j = 0; j < n; ++j)
                if (matrix[i][j])
                    f[i] += phi[Edge(i, j)];
        }

        // looking for duplicates
        sort(f.begin(), f.end());
        bool antimagic = true;
        for (int i = 0; i < n - 1; ++i)
            if (f[i] == f[i + 1]) {
                antimagic = false;
                break;
            }

        // return true if antimagic phi was found
        if (antimagic)
            return AntimagicResult{true, false};
    }
    // return false if antimagic phi was never found
    return AntimagicResult{false, false};
}

vector<uint> Graph::get_all_distances(Vertex u) {
    vector<uint> distances(n, INFINITE_DISTANCE);
    distances[u] = 0;
    queue<Vertex> q;
    q.push(u);

    while (!q.empty()) {
        Vertex t = q.front();
        q.pop();
        for (Vertex ti = 0; ti < n; ++ti)
            if (matrix[t][ti] == 1 && distances[ti] == INFINITE_DISTANCE) {
                q.push(ti);
                distances[ti] = distances[t] + 1;
            }
    }
    return distances;
}

bool Graph::is_connected() {
    vector<uint> distances = get_all_distances(0);
    for (uint d : distances)
        if (d == INFINITE_DISTANCE)
            return false;
    return true;
}
