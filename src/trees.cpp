#include <vector>
#include <algorithm>
#include <set>
#include <iostream>

#include "trees.h"


Vertices find_leaves(Graph* g) {
    Vertices res;
    for (int i = 0; i < g->n; ++i) {
        int sum = 0;
        for (int j = 0; j < g->n; ++j)
            if (g->matrix[i][j] == 1)
                sum += 1;
        if (sum == 1)
            res.push_back(i);
    }
    return res;
}


numeration_t numerate(Graph* g) {
    VecVertices adj = g->get_adj_list();
    Vertices leaves = find_leaves(g);

    phi_t phi;
    int current_phi = 1;

    for (int v : leaves) {
        Edge e = Edge(v, adj[v][0]);
        phi[e] = current_phi++;
    }

    Vertices leaves_next;
    for (Vertex v = 0; v < g->n; ++v) {
        for (Vertex leave : leaves)
            if (find(adj[v].begin(), adj[v].end(), leave) != adj[v].end()) {
                leaves_next.push_back(v);
                break;
            }
    }

    while (true) {
        for (int i = leaves_next.size() - 1; i >= 0; --i) {
            Vertex v = leaves_next[i];
            int count = 0;
            for (Vertex u : adj[v])
                if (find(leaves.begin(), leaves.end(), u) != leaves.end())
                    count++;
            if (count != adj[v].size() - 1)
                leaves_next.erase(leaves_next.begin() + i);
        }
        sort(leaves_next.begin(), leaves_next.end(), [leaves, adj, phi](const Vertex &v1, const Vertex &v2) -> bool {
            int f_v1 = 0;
            int f_v2 = 0;
            for (Vertex leave : leaves) {
                if (find(adj[v1].begin(), adj[v1].end(), leave) != adj[v1].end())
                    f_v1 += phi.at(Edge(v1, leave));
                else if (find(adj[v2].begin(), adj[v2].end(), leave) != adj[v2].end())
                    f_v2 += phi.at(Edge(v2, leave));
            }
            return f_v1 <= f_v2;
        });
        Vertices leaves_super_next;
        for (Vertex v : leaves_next) {
            for (Vertex u : adj[v])
                if (find(leaves.begin(), leaves.end(), u) == leaves.end()) {
                    phi[Edge(v, u)] = current_phi++;
                    leaves_super_next.push_back(u);
                    break;
                }
        }
        if (leaves_super_next.empty())
            break;

        leaves.clear();
        for (Vertex v : leaves_super_next) {
            for (Vertex u : adj[v])
                if (phi.find(Edge(u, v)) != phi.end())
                    leaves.push_back(u);
        }
        leaves_next = leaves_super_next;
        if (leaves_next.size() == 2) {
            Vertex u = leaves_next[0];
            Vertex v = leaves_next[1];
            if (find(adj[u].begin(), adj[u].end(), v) != adj[u].end()) {
                phi[Edge(u, v)] = current_phi;
                return numeration_t(phi, true);
            }
        }
    }
    return numeration_t(phi, false);
}


Vertex get_most_distant(Graph* g, Vertex u) {
    vector<uint> distances = g->get_all_distances(u);
    int max = INFINITE_DISTANCE;
    for (uint d : distances)
        if (d > max || max == INFINITE_DISTANCE)
            max = d;
    if (max == INFINITE_DISTANCE)
        throw runtime_error("Infinite distance to all vertices");
    for (int i = 0; i < distances.size(); ++i)
        if (distances[i] == max)
            return i;
    throw runtime_error("Undefined behaviour: get_most_distant");
}


VecVertices get_levels(Graph* g) {
    Vertex start = get_most_distant(g, 0);

    Vertex end = get_most_distant(g, start);
    Vertices max_path = g->get_path(start, end);

    uint diameter = max_path.size();
    uint radius = diameter / 2;

    VecVertices levels(radius);
    if (max_path.size() % 2 == 0) {
        Vertex center1 = max_path[radius];
        Vertex center2 = max_path[radius - 1];
        set<Vertex> center = {center2};
        vector<uint> distances1 = g->get_all_distances(center1, &center);
        center = {center1};
        vector<uint> distances2 = g->get_all_distances(center2, &center);

        for (Vertex v = 0; v < g->n; ++v) {
            if (distances1[v] != INFINITE_DISTANCE)
                levels[distances1[v]].push_back(v);
            else
                levels[distances2[v]].push_back(v);
        }
    } else {
        Vertex center = max_path[radius];
        vector<uint> distances = g->get_all_distances(center);
        for (Vertex v = 0; v < g->n; ++v)
            levels[distances[v]].push_back(v);
    }
    return levels;
}


numeration_t numerate2(Graph* g) {
    VecVertices adj = g->get_adj_list();

    VecVertices levels = get_levels(g);

    phi_t phi;
    int current_phi = 1;

    for (int v : levels[levels.size() - 1]) {
        Edge e = Edge(v, adj[v][0]);
        phi[e] = current_phi++;
    }

    for (int level = levels.size() - 2; level > 0; --level) {
        sort(levels[level].begin(), levels[level].end(), [adj, levels, phi, level](const Vertex &v1, const Vertex &v2) -> bool {
            int f_v1 = 0;
            int f_v2 = 0;
            for (Vertex child : levels[level + 1]) {
                if (find(adj[v1].begin(), adj[v1].end(), child) != adj[v1].end())
                    f_v1 += phi.at(Edge(v1, child));
                else if (find(adj[v2].begin(), adj[v2].end(), child) != adj[v2].end())
                    f_v2 += phi.at(Edge(v2, child));
            }
            return f_v1 < f_v2;
        });

        for (Vertex v : levels[level]) {
            for (Vertex ancestor : levels[level - 1])
                if (find(adj[v].begin(), adj[v].end(), ancestor) != adj[v].end()) {
                    phi[Edge(v, ancestor)] = current_phi++;
                    break;
                }
        }
    }
    if (levels[0].size() == 2)
        phi[Edge(levels[0][0], levels[0][1])] = current_phi;

    return pair<phi_t, bool>(phi, levels[0].size() == 2);
}


bool test_numeration(Graph* g, const map<Edge, int>& phi) {
    set<int> f;
    for (Vertex v = 0; v < g->n; ++v) {
        int f_v = 0;
        for (Vertex u = 0; u < g->n; ++u)
            if (g->matrix[v][u] == 1)
                f_v += phi.at(Edge(v, u));
        if (f.find(f_v) != f.end())
            return false;
        f.insert(f_v);
    }
    return true;
}