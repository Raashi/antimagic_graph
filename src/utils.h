#ifndef ANTIMAGIC_UTILS_H
#define ANTIMAGIC_UTILS_H

#include <string>
#include <vector>
#include <random>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;


const string SYS_MSG   = "[ SYS ] "; // NOLINT(cert-err58-cpp)
const string ERROR_MSG = "[ERROR] "; // NOLINT(cert-err58-cpp)

int string_to_int(string &);

bool has_arg(int argc, char **argv, const string& arg);

string get_arg(int, char **, const string&, string /* default */);
int    get_arg(int, char **, const string&, int);
ulong  get_arg(int, char **, string, ulong);

void write_to_file(const string&, vector<string>);

#endif //ANTIMAGIC_UTILS_H
