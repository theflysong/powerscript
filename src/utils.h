#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Alloc a [type] variable
#define NEW(type) ((type *)malloc(sizeof(type)))
char *str_slice(char *buffer, int start, int end);

#endif