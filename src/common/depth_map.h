#ifndef CLYRA_SCOPE_H
#define CLYRA_SCOPE_H

#include <parser/ast.h>
#include "common/hashmap.h"

typedef map_t NodeDepthMap;

NodeDepthMap* newNodeDepthMap();

int nodeDepthMapAdd(NodeDepthMap* ndm, Node** node, int* depth);

int nodeDepthMapGet(NodeDepthMap* ndm, Node** node, int* outDepth);

void nodeDepthMapFree(NodeDepthMap* ndm);

#endif //CLYRA_SCOPE_H
