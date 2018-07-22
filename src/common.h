#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include "csim.h"

void Log(char*);

void Error(char*);

void* Malloc(size_t);

#endif
