#ifndef CLYRA_AST_H
#define CLYRA_AST_H

#include <stdbool.h>
#include <typechecker/types.h>

#include "lexer/lexer.h"

#ifdef C
#undef C
#endif
#define C(ENUM_VAL) ENUM_VAL,
#define AST_NODE_TYPES \
    C(NODE_TYPE_LITERAL) \
    C(NODE_TYPE_ARRAY_LITERAL) \
    C(NODE_TYPE_OBJECT_LITERAL) \
    C(NODE_TYPE_IDENT) \
    C(NODE_TYPE_UNARY) \
    C(NODE_TYPE_BINARY) \
    C(NODE_TYPE_GROUPING) \
    C(NODE_TYPE_IF_ELSE) \
    C(NODE_TYPE_BLOCK) \
    C(NODE_TYPE_INVOCATION) \
    C(NODE_TYPE_VAL_DECL_STATEMENT) \
    C(NODE_TYPE_FUNC_DECL_STATEMENT) \
    C(NODE_TYPE_TYPE_DECL_STATEMENT)

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

typedef struct {
    Node* ident;
    Node* value;
} ObjectLiteralEntry;
typedef struct {
    Token* token;
    ObjectLiteralEntry** entries;
    Node** keys;
    int size;
} ObjectLiteralNode;

typedef struct {
    Token* token;
    Node* expr;
} GroupingNode;

typedef struct {
    Token* token;
    Node** exprs;
    int numExprs;
} BlockNode;

// An if-else node can either be a statement or expression
// If it's an expression the else block is required
typedef struct {
    Token* token;
    Node* conditionExpr;
    Node* thenExpr;
    Node* elseExpr;
} IfElseNode;

typedef struct {
    Token* token;
    Node* target;
    int numArgs;
    Node** arguments;
    char** argNames;
} InvocationNode;

// ------------------------------------
//             Statements
// ------------------------------------

typedef struct TypeExpr TypeExpr;

typedef struct {
    Token* token;
    IdentifierNode* ident;
    TypeExpr* typeAnnotation;
    Node* assignment;
    bool isMutable;
} ValDeclStmt;

typedef struct {
    Token* token;
    IdentifierNode* name;
    int numParams;
    Node** params;
    TypeExpr** paramTypeAnnotations;
    Node* body;
    TypeExpr* returnTypeAnnotation;
} FuncDeclStmt;

typedef struct {
    Token* token;
    IdentifierNode* name;
    TypeExpr* typeExpr;
    int numArgs;
    IdentifierNode** typeArgs;
} TypeDeclStmt;

// ------------------------------------
//             Type Node
// ------------------------------------

#undef C
#define C(ENUM_VAL) ENUM_VAL,
#define TYPE_EXPR_TYPES \
    C(TYPE_EXPR_BASIC_TYPE) \
    C(TYPE_EXPR_TUPLE_TYPE) \
    C(TYPE_EXPR_STRUCT_TYPE) \
    C(TYPE_EXPR_ENUM_TYPE)

typedef enum {
    TYPE_EXPR_TYPES
} TypeExprType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* typeExprTypes[];

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
    Type type;
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
