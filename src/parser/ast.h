#ifndef CLYRA_AST_H
#define CLYRA_AST_H

#include <stdbool.h>

#include "typechecker/types.h"
#include "lexer/lexer.h"
#include "common/enums.h"

MAKE_ENUM(
    AstNodeType,
    astNodeTypes,

    NODE_TYPE_LITERAL,
    NODE_TYPE_ARRAY_LITERAL,
    NODE_TYPE_OBJECT_LITERAL,
    NODE_TYPE_IDENT,
    NODE_TYPE_UNARY,
    NODE_TYPE_BINARY,
    NODE_TYPE_GROUPING,
    NODE_TYPE_IF_ELSE,
    NODE_TYPE_BLOCK,
    NODE_TYPE_INVOCATION,
    NODE_TYPE_VAL_DECL_STATEMENT,
    NODE_TYPE_FUNC_DECL_STATEMENT,
    NODE_TYPE_TYPE_DECL_STATEMENT
)

typedef struct Node Node;

MAKE_ENUM(
    LiteralNodeType,
    literalNodeTypes,

    LITERAL_NODE_INT,
    LITERAL_NODE_DOUBLE,
    LITERAL_NODE_STRING,
    LITERAL_NODE_BOOL,
    LITERAL_NODE_NIL
)

// ------------------------------------
//             Expressions
// ------------------------------------

// HACK! This only works because the first field in all of the union structs of Node is Token*
#define NODE_GET_TOKEN_HACK(node) (node->as.literalNode->token)

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
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
    Token* token; // See NODE_GET_TOKEN_HACK
    const char* name;
} IdentifierNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node* expr;
} UnaryNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node* lExpr;
    Node* rExpr;
} BinaryNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node** elements;
    int size;
} ArrayLiteralNode;

typedef struct {
    Node* ident;
    Node* value;
} ObjectLiteralEntry;
typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    ObjectLiteralEntry** entries;
    Node** keys;
    int size;
} ObjectLiteralNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node* expr;
} GroupingNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node** exprs;
    int numExprs;
} BlockNode;

// An if-else node can either be a statement or expression
// If it's an expression the else block is required
typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node* conditionExpr;
    Node* thenExpr;
    Node* elseExpr;
} IfElseNode;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    Node* target;
    int numArgs;
    Node** arguments;
    char** argNames;
} InvocationNode;

// ------------------------------------
//             Statements
// ------------------------------------

typedef struct TypeExpr TypeExpr; // Ignore warning: harmless redefine from forward-decl in types.h

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    IdentifierNode* ident;
    TypeExpr* typeAnnotation;
    Node* assignment;
    bool isMutable;
} ValDeclStmt;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    IdentifierNode* name;
    int numParams;
    Node** params;
    TypeExpr** paramTypeAnnotations;
    Node* body;
    TypeExpr* returnTypeAnnotation;
} FuncDeclStmt;

typedef struct {
    Token* token; // See NODE_GET_TOKEN_HACK
    IdentifierNode* name;
    TypeExpr* typeExpr;
    int numArgs;
    IdentifierNode** typeArgs;
} TypeDeclStmt;

// ------------------------------------
//             Type Node
// ------------------------------------

MAKE_ENUM(
    TypeExprType,
    typeExprTypes,

    TYPE_EXPR_BASIC_TYPE,
    TYPE_EXPR_TUPLE_TYPE,
    TYPE_EXPR_STRUCT_TYPE,
    TYPE_EXPR_ENUM_TYPE
)

struct TypeExpr {
    Token* token;
    TypeExprType type;
    int numArgs;
    TypeExpr** typeArgs;
    union {
        struct {
            IdentifierNode* name;
        } basicType;
        struct {
            int numFields;
            Node** keys;
            TypeExpr** fields;
        } structType;
        struct {
            int numOptions;
            TypeExpr** options;
        } enumType;
    } as;
};

TypeExpr* newBasicTypeExpr(Token* token, IdentifierNode* name, int numArgs, TypeExpr** typeArgs);

TypeExpr* newTupleTypeExpr(Token* token, int numArgs, TypeExpr** typeArgs);

TypeExpr* newStructTypeExpr(Token* token, int numFields, Node** keys, TypeExpr** fields);

TypeExpr* newEnumTypeExpr(Token* token, int numOptions, TypeExpr** options);

// ------------------------------------
//             Base Node
// ------------------------------------

struct Node {
    AstNodeType nodeType;
    Type* type;
    union {
        ValDeclStmt* valDeclStmt;
        FuncDeclStmt* funcDeclStmt;
        TypeDeclStmt* typeDeclStmt;
        LiteralNode* literalNode;
        ArrayLiteralNode* arrayLiteralNode;
        ObjectLiteralNode* objectLiteralNode;
        IdentifierNode* identifierNode;
        UnaryNode* unaryNode;
        BinaryNode* binaryNode;
        GroupingNode* groupingNode;
        IfElseNode* ifElseNode;
        BlockNode* blockNode;
        InvocationNode* invocationNode;
    } as;
};

Node* newLiteralNode(Token* token);

Node* newIdentifierNode(Token* token);

Node* newUnaryNode(Token* token, Node* expr);

Node* newBinaryNode(Token* token, Node* lExpr, Node* rExpr);

Node* newArrayLiteralNode(Token* token, Node** elements, int size);

ObjectLiteralEntry* newObjectLiteralEntry(Node* key, Node* value);

Node* newObjectLiteralNode(Token* token, ObjectLiteralEntry** entries, Node** keys, int size);

Node* newGroupingNode(Token* token, Node* expr);

Node* newIfElseNode(Token* token, Node* condExpr, Node* thenExpr, Node* elseExpr);

Node* newBlockNode(Token* token, Node** exprs, int numExprs);

Node* newInvocationNode(Token* token, Node* target, int numArgs, Node** arguments, char** argNames);

Node* newValDeclStmtNode(Token* token, Node* identNode, TypeExpr* typeAnnot, Node* assignment, bool isMutable);

Node* newFuncDeclStmtNode(Token* token, Node* nameNode, int numParams, Node** params, TypeExpr** paramTypeAnnots, Node* body, TypeExpr* optRetTypeAnnot);

Node* newTypeDeclStmtNode(Token* token, IdentifierNode* name, TypeExpr* typeExpr, int numArgs, IdentifierNode** typeArgs);

#endif //CLYRA_AST_H
