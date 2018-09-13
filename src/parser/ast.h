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
    C(NODE_TYPE_IDENT) \
    C(NODE_TYPE_VAL_DECL_STATEMENT)

typedef enum {
    AST_NODE_TYPES
} AstNodeType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* astNodeTypes[];

typedef struct Node Node;
typedef struct IdentifierNode IdentifierNode;

typedef struct {
    Token* token;
    IdentifierNode* ident;
    Node* assignment;
} ValDeclStmt;

#undef C
#define C(ENUM_VAL) ENUM_VAL,
#define LITERAL_NODE_TYPES \
    C(LITERAL_NODE_INT) \
    C(LITERAL_NODE_DOUBLE) \
    C(LITERAL_NODE_STRING) \
    C(LITERAL_NODE_BOOL) \
    C(LITERAL_NODE_NIL)

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

struct IdentifierNode {
    Token* token;
    const char* name;
};

struct Node {
    AstNodeType type;
    union {
        ValDeclStmt* valDeclStmt;
        LiteralNode* literalNode;
        IdentifierNode* identifierNode;
    } as;
};

Node* newValDeclStmtNode(Token* token, Node* identNode, Node* assignment);

Node* newLiteralNode(Token* token);

Node* newIdentifierNode(Token* token);

#endif //CLYRA_AST_H
