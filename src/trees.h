#ifndef ANTIMAGIC_TREES_H
#define ANTIMAGIC_TREES_H

#include <map>

#include "utils.h"
#include "graph.h"

using namespace std;

typedef pair<phi_t, bool> numeration_t;

VecVertices get_levels(Graph* g);
numeration_t numerate(Graph*);

bool test_numeration(Graph*, const map<Edge, int>&);

#endif //ANTIMAGIC_TREES_H
