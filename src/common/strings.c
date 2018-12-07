#include <string.h>

#include "strings.h"

char* substring(const char* str, size_t len) {
    char* buf = malloc(len + 1);
    strncpy(buf, str, len);
    buf[len] = 0;
    return buf;
}
