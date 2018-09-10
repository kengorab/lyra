#ifndef CLYRA_MEM_CHECKS_H
#define CLYRA_MEM_CHECKS_H

#include <stdlib.h>
#include <stdio.h>

void* my_malloc(size_t size, const char* file, int line) {
    void* p = malloc(size);
    printf("Allocated %p (%s:%i)\n", p, file, line);
    return p;
}

#define malloc(X) my_malloc(X, __FILE__, __LINE__)

void my_free(void* memory, const char* file, int line) {
    free(memory);
    printf("Freed     %p (%s:%i)\n", memory, file, line);
}

#define free(X) my_free(X, __FILE__, __LINE__)


#endif //CLYRA_MEM_CHECKS_H
