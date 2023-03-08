#include <error.h>
#include <stdarg.h>

void warnf(const char *callee, const char *type, const char *fmt, ...) {
    va_list lst;
    va_start(lst, fmt);

    char buffer[512];

    // format the message
    vsprintf(buffer, fmt, lst);

    // output the warning message
    printf("(%s)[WARN]%s: %s\n", callee, type, buffer);

    va_end(lst);
}

void errorf(const char *callee, const char *type, const char *fmt, ...) {
    va_list lst;
    va_start(lst, fmt);

    char buffer[512];

    // format the message
    vsprintf(buffer, fmt, lst);

    // output the error message
    printf("(%s)[ERROR]%s: %s\n", callee, type, buffer);

    va_end(lst);

    // terminate the program
    exit(-1);
}