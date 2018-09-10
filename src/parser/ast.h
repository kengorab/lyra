#ifndef CLYRA_AST_H
#define CLYRA_AST_H

#include <stdbool.h>

#include "lexer.h"

#ifdef C
#undef C
#endif
#define C(ENUM_VAL) ENUM_VAL,
#define AST_NODE_TYPES \
    C(NODE_TYPE_LITERAL) \
    C(NODE_TYPE_IDENT)

typedef enum {
    AST_NODE_TYPES
} AstNodeType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* astNodeTypes[];

typedef struct Node Node;

#undef C
#define C(ENUM_VAL) ENUM_VAL,
#define LITERAL_NODE_TYPES \
    C(LITERAL_NODE_INT) \
    C(LITERAL_NODE_DOUBLE) \
    C(LITERAL_NODE_STRING) \
    C(LITERAL_NODE_BOOL)

typedef enum {
    LITERAL_NODE_TYPES
} LiteralNodeType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* literalNodeTypes[];

typedef struct {
    Token* token;
    LiteralNodeType type;
    union {
        int iVal;
        double dVal;
        const char* strVal;
        bool bVal;
    };
} LiteralNode;

typedef struct {
    Token* token;
    const char* name;
} IdentifierNode;

//typedef struct {
//    Token token;
//    Node* lExpr;
//    char* op;
//    Node* rExpr;
//} BinaryNode;

struct Node {
    AstNodeType type;
    union {
        LiteralNode* literalNode;
        IdentifierNode* identifierNode;
//        BinaryNode* binaryNode;
    } as;
};

Node* newLiteralNode(Token* token);

Node* newIdentifierNode(Token* token);

#endif //CLYRA_AST_H
