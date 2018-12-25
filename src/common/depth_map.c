#include <stdlib.h>
#include <stdio.h>

#include "depth_map.h"

/*
 This is super dirty, but it's a Map<Node, int>, using the toString-ed memory address of the Node as a hash key
 */

NodeDepthMap* newNodeDepthMap() {
    return hashmap_new();
}

int nodeDepthMapAdd(NodeDepthMap* ndm, Node** node, int* depth) {
    char* buf = malloc(20);
    snprintf(buf, 20, "%ld", node);
    printf("%s\n", buf);

    return hashmap_put(ndm, buf, depth);
}

int nodeDepthMapGet(NodeDepthMap* ndm, Node** node, int* outDepth) {
    char* buf = malloc(20);
    snprintf(buf, 20, "%ld", node);

    return hashmap_get(ndm, buf, outDepth);
}

void nodeDepthMapFree(NodeDepthMap* ndm) {
    hashmap_free(ndm);
}

