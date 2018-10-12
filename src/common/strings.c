#include <string.h>

#include "strings.h"

char* substring(const char* str, size_t len) {
    char* buf = malloc(len + 1);
    strncpy(buf, str, len);
    return buf;
}
