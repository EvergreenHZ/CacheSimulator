#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include "csim.h"

#define LOG(...) \
        printf(__VA_ARGS__)
void Log(char*);

void Error(char*);

void* Malloc(size_t);

//unsigned long long randNum(unsigned long long module);

#endif
