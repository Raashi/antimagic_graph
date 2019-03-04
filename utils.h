#ifndef ANTIMAGIC_UTILS_H
#define ANTIMAGIC_UTILS_H

#include <string>

void print_perm(const int* perm, int n);

int graph6_to_n(std::string & graph6);
int** graph6_to_matrix(std::string & graph6);
int** graph6_to_list(std::string & graph6);

#endif //ANTIMAGIC_UTILS_H
