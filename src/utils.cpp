#include <sstream>
#include <exception>
#include <string>
#include <stdlib.h>

#include "utils.h"

using namespace std;


int string_to_int(string & value) {
    return stoi(value);
}


double string_to_double(string & value) {
    return atof(value.c_str()); // NOLINT(cert-err34-c)
}


bool has_arg(int argc, char **argv, string arg) {
    for (int i = 0; i < argc; ++i) {
        string opt = argv[i];
        if (opt == arg)
            return true;
    }
    return false;
}


string get_arg(int argc, char **argv, string arg, string def) {
    for (int i = 0; i < argc; ++i) {
        string opt = argv[i];
        if (opt == arg) {
            if (i + 1 == argc)
                throw "No value for argument '" + arg + "'";
            string value = argv[i + 1];
            return value;
        }
    }
    return def;
}


int get_arg(int argc, char **argv, string arg, int def) {
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


double get_arg(int argc, char **argv, string arg, double def) {
    string value = get_arg(argc, argv, arg, ""); // NOLINT(performance-unnecessary-value-param)
    if (value.empty())
        return def;

    try {
        return string_to_double(value);
    } catch (runtime_error& error) {
        puts((ERROR_MSG + "Error when taking argument double-value for '" + arg + "'").c_str());
        throw runtime_error(ERROR_MSG + error.what());
    }
}