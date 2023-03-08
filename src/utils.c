#include <utils.h>

// Slice a string
char *str_slice(char *buffer, int start, int end) {
    char *slice = (char *)malloc(end - start + 1);
    memcpy(slice, buffer + start, end - start);
    slice[end - start] = '\0';
    return slice;
}