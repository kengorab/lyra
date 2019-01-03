#ifndef CLYRA_TOJSON_VISITOR_H
#define CLYRA_TOJSON_VISITOR_H

#include "common/list.h"
#include "typechecker/typechecker.h"

void toJson_visit(const char* filePath, Typechecker* tc);

#endif //CLYRA_TOJSON_VISITOR_H
