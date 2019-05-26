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

int string_to_int(string &value) {
    return stoi(value);
}

string get_arg(int argc, char **argv, const string &arg, string def) {
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

int get_arg(int argc, char **argv, const string &arg, int def) {
    string value = get_arg(argc, argv, arg, "");
    if (value.empty())
        return def;
    try {
        return string_to_int(value);
    } catch (runtime_error &error) {
        puts((ERROR_MSG + "Error when taking argument int-value for '" + arg + "'").c_str());
        throw runtime_error(ERROR_MSG + error.what());
    }
}

uint get_arg(int argc, char **argv, const string &arg, uint def) {
    int res = get_arg(argc, argv, arg, int(def));
    return ulong(res);
}

void write_to_file(const string &filename, vector<string> data) {
    ofstream output_file(filename);
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(data.begin(), data.end(), output_iterator);
}

ullong get_file_size(const char *filename) {
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA file_info;
    if (!GetFileAttributesEx(filename, GetFileExInfoStandard, (void *) &file_info))
        return -1;
    uint32_t hi = file_info.nFileSizeHigh, lo = file_info.nFileSizeLow;
    uint64_t file_size = (static_cast<uint64_t>(hi) << 32ull) | lo;
    return file_size;
#else
    struct stat stat_buf{};
    FILE* fp = fopen(filename, "rb");
    int rc = fstat(fileno(fp), &stat_buf);
    ulong res = stat_buf.st_size;
    fclose(fp);
    return res;
#endif
}

ullong get_graph_count(char *filename) {
    ullong file_size = get_file_size(filename);
    ifstream file(filename);
    string first_line;
    getline(file, first_line);
    return file_size / (static_cast<uint64_t>(first_line.length()) + 1ull);
}