#ifndef ANTIMAGIC_BRUTE_H
#define ANTIMAGIC_BRUTE_H

#include <fstream>
#include <windows.h>

#include "utils.h"

const ulong THREAD_COUNT_DEFAULT = 4;

void brute(int argc, char** argv, ifstream* fp);

#endif //ANTIMAGIC_BRUTE_H
