#ifndef CLYRA_MAPS_H
#define CLYRA_MAPS_H

#include "hashmap.h"

#define CREATE_HASHMAP_DECL(mapName, valueType) \
    typedef map_t mapName; \
    \
    mapName* new##mapName(); \
    \
    int add_##mapName(mapName* map, const char* key, valueType* value); \
    \
    int get_##mapName(mapName* map, const char* key, valueType** outValue); \
    \
    void free_##mapName(mapName* map);

#define CREATE_HASHMAP_IMPL(mapName, valueType) \
    mapName* new##mapName() { \
        return hashmap_new(); \
    } \
    \
    int add_##mapName(mapName* map, const char* key, valueType* value) { \
        return hashmap_put(map, (char*) key, (void*) value); \
    } \
    \
    int get_##mapName(mapName* map, const char* key, valueType** outValue) { \
        return hashmap_get(map, (char*) key, (void**) outValue); \
    } \
    \
    void free_##mapName(mapName* map) { \
        hashmap_free(map); \
    }

#endif //CLYRA_MAPS_H
