#ifndef ANTIMAGIC_UTILS_H
#define ANTIMAGIC_UTILS_H

#include <string>
#include <windows.h>

using namespace std;

const string SYS_MSG   = "[ SYS ] "; // NOLINT(cert-err58-cpp)
const string ERROR_MSG = "[ERROR] "; // NOLINT(cert-err58-cpp)

int string_to_int(string &);
double string_to_double(string &);

bool has_arg(int argc, char **argv, string arg);

string get_arg(int, char **, string, string /* default */);
int    get_arg(int, char **, string, int);
DWORD  get_arg(int, char **, string, DWORD);
double get_arg(int, char **, string, double);

#endif //ANTIMAGIC_UTILS_H
