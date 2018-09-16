#ifndef CLYRA_AST_H
#define CLYRA_AST_H

#include <stdbool.h>

#include "lexer/lexer.h"

#ifdef C
#undef C
#endif
#define C(ENUM_VAL) ENUM_VAL,
#define AST_NODE_TYPES \
    C(NODE_TYPE_LITERAL) \
    C(NODE_TYPE_ARRAY_LITERAL) \
    C(NODE_TYPE_IDENT) \
    C(NODE_TYPE_UNARY) \
    C(NODE_TYPE_BINARY) \
    C(NODE_TYPE_VAL_DECL_STATEMENT)

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
    C(LITERAL_NODE_BOOL) \
    C(LITERAL_NODE_NIL)

typedef enum {
    LITERAL_NODE_TYPES
} LiteralNodeType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* literalNodeTypes[];

// ------------------------------------
//             Expressions
// ------------------------------------

typedef struct {
    Token* token;
    LiteralNodeType type;
    union {
        int iVal;
        double dVal;
        struct {
            const char* val;
            int length;
        } str;
        bool bVal;
    };
} LiteralNode;

typedef struct {
    Token* token;
    const char* name;
} IdentifierNode;

typedef struct {
    Token* token;
    Node* expr;
} UnaryNode;

typedef struct {
    Token* token;
    Node* lExpr;
    Node* rExpr;
} BinaryNode;

typedef struct {
    Token* token;
    Node** elements;
    int size;
} ArrayLiteralNode;

// ------------------------------------
//             Statements
// ------------------------------------

typedef struct {
    Token* token;
    IdentifierNode* ident;
    Node* assignment;
} ValDeclStmt;

// ------------------------------------
//             Base Node
// ------------------------------------

struct Node {
    AstNodeType type;
    union {
        ValDeclStmt* valDeclStmt;
        LiteralNode* literalNode;
        ArrayLiteralNode* arrayLiteralNode;
        IdentifierNode* identifierNode;
        UnaryNode* unaryNode;
        BinaryNode* binaryNode;
    } as;
};

Node* newLiteralNode(Token* token);

Node* newIdentifierNode(Token* token);

Node* newUnaryNode(Token* token, Node* expr);

Node* newBinaryNode(Token* token, Node* lExpr, Node* rExpr);

Node* newArrayLiteralNode(Token* token, Node** elements, int size);

Node* newValDeclStmtNode(Token* token, Node* identNode, Node* assignment);

#endif //CLYRA_AST_H
