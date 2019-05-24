#include <sstream>
#include <exception>
#include <fstream>
#include <iterator>
#include <stdlib.h>

#include "utils.h"


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

ulong get_arg(int argc, char **argv, string arg, ulong def) {
    int res = get_arg(argc, argv, arg, int(def)); // NOLINT(performance-unnecessary-value-param)
    return ulong(res);
}

void write_to_file(const string& filename, vector<string> data) {
    ofstream output_file(filename);
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(data.begin(), data.end(), output_iterator);
}