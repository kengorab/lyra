#ifndef CLYRA_TYPEDEFS_H
#define CLYRA_TYPEDEFS_H

#include "common/maps.h"

typedef struct Type Type;

// declare TypesMap = Map<const char*, Type>
CREATE_HASHMAP_DECL(TypesMap, Type);

#endif //CLYRA_TYPEDEFS_H
