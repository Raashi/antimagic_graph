#ifndef ANTIMAGIC_UTILS_H
#define ANTIMAGIC_UTILS_H

#include <string>
#include <vector>
#include <random>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

const string SYS_MSG   = "[ SYS ] "; // NOLINT(cert-err58-cpp)
const string ERROR_MSG = "[ERROR] "; // NOLINT(cert-err58-cpp)

struct Mutex {
#ifdef _WIN32
    CRITICAL_SECTION cs{};
#else
    pthread_mutex_t mutex{};
#endif

    Mutex ();
    ~Mutex();

    void lock();
    void unlock();
};


int string_to_int(string &);

bool has_arg(int argc, char **argv, const string& arg);

string get_arg(int, char **, const string&, string /* default */);
int    get_arg(int, char **, const string&, int);
uint   get_arg(int, char **, const string&, uint);

void write_to_file(const string&, vector<string>);

ulong get_graph_count(string filename);

#endif //ANTIMAGIC_UTILS_H
