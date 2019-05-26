#include <sstream>
#include <exception>
#include <fstream>
#include <iterator>
#include <stdlib.h>
#include <sys/stat.h>

#include "utils.h"

Mutex::Mutex() {
#ifdef _WIN32
    InitializeCriticalSection(&this->cs);
#else
    pthread_mutex_init(&this->mutex, nullptr);
#endif
}

Mutex::~Mutex() {
#ifdef _WIN32
    DeleteCriticalSection(&this->cs);
#else
    pthread_mutex_destroy(&this->mutex);
#endif
}

void Mutex::lock() {
#ifdef _WIN32
    EnterCriticalSection(&this->cs);
#else
    pthread_mutex_lock(&this->mutex);
#endif
}

void Mutex::unlock() {
#ifdef _WIN32
    LeaveCriticalSection(&this->cs);
#else
    pthread_mutex_unlock(&this->mutex);
#endif
}

int string_to_int(string & value) {
    return stoi(value);
}

bool has_arg(int argc, char **argv, const string& arg) {
    for (int i = 0; i < argc; ++i) {
        string opt = argv[i];
        if (opt == arg)
            return true;
    }
    return false;
}

string get_arg(int argc, char **argv, const string& arg, string def) {
    for (int i = 0; i < argc; ++i) {
        string opt = argv[i];
        if (opt == arg) {
            if (i + 1 == argc)
                throw runtime_error("No value for argument '" + arg + "'");
            string value = argv[i + 1];
            return value;
        }
    }
    return def;
}

int get_arg(int argc, char **argv, const string& arg, int def) {
    string value = get_arg(argc, argv, arg, "");
    if (value.empty())
        return def;
    try {
        return string_to_int(value);
    } catch (runtime_error& error) {
        puts((ERROR_MSG + "Error when taking argument int-value for '" + arg + "'").c_str());
        throw runtime_error(ERROR_MSG + error.what());
    }
}

uint get_arg(int argc, char **argv, const string& arg, uint def) {
    int res = get_arg(argc, argv, arg, int(def));
    return ulong(res);
}

void write_to_file(const string& filename, vector<string> data) {
    ofstream output_file(filename);
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(data.begin(), data.end(), output_iterator);
}

ulong get_file_size(string filename) {
    struct stat64 stat_buf{};
    int rc = stat64(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

ulong get_graph_count(string filename) {
    ulong file_size = get_file_size(filename);
    ifstream file(filename);
    string first_line;
    getline(file, first_line);
    return file_size / (first_line.length() + 1);
}