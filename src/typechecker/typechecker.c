#include <stdio.h>
#include <stdlib.h>
#include <parser/ast.h>

#include "parser/ast.h"
#include "typechecker.h"
#include "common/stack.h"
#include "common/hashmap.h"

void beginScope(Typechecker* tc);

void endScope(Typechecker* tc);

static TypecheckError* visit(Typechecker* tc, Node* node);

static TypecheckError* visitTypeExpr(Typechecker* tc, TypeExpr* typeExpr);

static TypecheckError* visitLiteralNode(Typechecker* tc, Node* node) {
    LiteralNode* literalNode = node->as.literalNode;
    switch (literalNode->type) {
        case LITERAL_NODE_STRING: {
            node->type = typeString;
            break;
        }
        case LITERAL_NODE_INT: {
            node->type = typeInt;
            break;
        }
        case LITERAL_NODE_DOUBLE: {
            node->type = typeDouble;
            break;
        }
        case LITERAL_NODE_BOOL: {
            node->type = typeBool;
            break;
        }
        case LITERAL_NODE_NIL: {
            node->type = typeNil;
            break;
        }
    }

    return NULL;
}

static TypecheckError* visitUnaryNode(Typechecker* tc, Node* node) {
    UnaryNode* unaryNode = node->as.unaryNode;
    TypecheckError* err = visit(tc, unaryNode->expr);
    if (err != NULL) return err;

    switch (unaryNode->token->type) {
        case TOKEN_MINUS: {
            if (!NODE_IS_NUMERIC(unaryNode->expr)) {
                return newTypecheckError(unaryNode->token, unaryNode->expr->type, 2, typeInt, typeDouble);
            }
            node->type = unaryNode->expr->type;
            break;
        }
        case TOKEN_BANG: {
            if (!NODE_IS_BOOL(unaryNode->expr)) {
                return newTypecheckError(unaryNode->token, unaryNode->expr->type, 1, typeBool);
            }
            node->type = typeBool;
            break;
        }
        default: {
            // Invalid state
            break;
        }
    }

    return NULL;
}

static TypecheckError* visitBinaryNode(Typechecker* tc, Node* node) {
    BinaryNode* binaryNode = node->as.binaryNode;
    TypecheckError* err = visit(tc, binaryNode->lExpr);
    if (err != NULL) return err;

    err = visit(tc, binaryNode->rExpr);
    if (err != NULL) return err;

    switch (binaryNode->token->type) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_STAR:
        case TOKEN_SLASH: {
            if (binaryNode->token->type == TOKEN_PLUS) {
                if (NODE_IS_STRING(binaryNode->lExpr) || NODE_IS_STRING(binaryNode->rExpr)) {
                    node->type = typeString;
                    return NULL;
                }
            }

            if (!NODE_IS_NUMERIC(binaryNode->lExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->lExpr->type, 2, typeInt, typeDouble);
            } else if (!NODE_IS_NUMERIC(binaryNode->rExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->rExpr->type, 2, typeInt, typeDouble);
            }

            if (binaryNode->token->type == TOKEN_SLASH) {
                node->type = typeDouble;
                return NULL;
            }

            if (NODE_IS_INT(binaryNode->lExpr) && NODE_IS_INT(binaryNode->rExpr)) {
                node->type = typeInt;
                return NULL;
            }

            node->type = typeDouble;
            return NULL;
        }
        case TOKEN_LT:
        case TOKEN_LTE:
        case TOKEN_GT:
        case TOKEN_GTE: {
            if (NODE_IS_NUMERIC(binaryNode->lExpr) && NODE_IS_NUMERIC(binaryNode->rExpr)) {
                node->type = typeBool;
                return NULL;
            } else if (!NODE_IS_NUMERIC(binaryNode->lExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->lExpr->type, 2, typeInt, typeDouble);
            } else if (!NODE_IS_NUMERIC(binaryNode->rExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->rExpr->type, 2, typeInt, typeDouble);
            }
        }
        case TOKEN_EQ_EQ:
        case TOKEN_BANG_EQ: {
            node->type = typeBool;
            return NULL;
        }
        case TOKEN_AND:
        case TOKEN_OR: {
            if (NODE_IS_BOOL(binaryNode->lExpr) && NODE_IS_BOOL(binaryNode->rExpr)) {
                node->type = typeBool;
                return NULL;
            } else if (!NODE_IS_BOOL(binaryNode->lExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->lExpr->type, 1, typeBool);
            } else if (!NODE_IS_BOOL(binaryNode->rExpr)) {
                return newTypecheckError(binaryNode->token, binaryNode->rExpr->type, 1, typeBool);
            }
        }
        default: {
            // Invalid state
            return NULL;
        }
    }
}

static TypecheckError* visitIdentifierNode(Typechecker* tc, Node* node) {
    IdentifierNode* identifierNode = node->as.identifierNode;
    return NULL;
}

static TypecheckError* visitIfElseNode(Typechecker* tc, Node* node) {
    IfElseNode* ifElseNode = node->as.ifElseNode;
    visit(tc, ifElseNode->conditionExpr);
    if (!NODE_IS_BOOL(ifElseNode->conditionExpr)) {
        // HACK, this only works because the first field in all of the union structs is Token*
        Token* token = ifElseNode->conditionExpr->as.literalNode->token;
        return newTypecheckError(token, ifElseNode->conditionExpr->type, 1, typeBool);
    }

    visit(tc, ifElseNode->thenExpr);
    if (ifElseNode->elseExpr != NULL) {
        visit(tc, ifElseNode->elseExpr);

        if (ifElseNode->thenExpr->type.type == ifElseNode->elseExpr->type.type) {
            node->type = ifElseNode->thenExpr->type;
        }
    }
    // TODO: If there is no else, the type of the Node should be Opt[THEN_TYPE]
    // TODO: If the then and else branches don't match, the type of the Node should be an Either[THEN_TYPE, ELSE_TYPE]
    return NULL;
}

static TypecheckError* visitBlockNode(Typechecker* tc, Node* node) {
    BlockNode* blockNode = node->as.blockNode;
    printf("%s\n", "visitBlockNode");
    for (int i = 0; i < blockNode->numExprs; ++i) {
        visit(tc, blockNode->exprs[i]);
    }
    return NULL;
}

static TypecheckError* visitInvocationNode(Typechecker* tc, Node* node) {
    InvocationNode* invocationNode = node->as.invocationNode;
    printf("%s\n", "visitInvocationNode");
    visit(tc, invocationNode->target);
    for (int i = 0; i < invocationNode->numArgs; ++i) {
        visit(tc, invocationNode->arguments[i]);
    }
    return NULL;
}

static TypecheckError* visitValDeclStmtNode(Typechecker* tc, Node* node) {
    ValDeclStmt* valDeclStmt = node->as.valDeclStmt;
    printf("%s\n", "visitValDeclStmtNode");
    visit(tc, valDeclStmt->assignment);
    return NULL;
}

static TypecheckError* visitFuncDeclStmtNode(Typechecker* tc, Node* node) {
    FuncDeclStmt* funcDeclStmt = node->as.funcDeclStmt;
    printf("%s\n", "visitFuncDeclStmtNode");
    visit(tc, funcDeclStmt->body);
    return NULL;
}

static TypecheckError* visitTypeExpr(Typechecker* tc, TypeExpr* typeExpr) {
    printf("%s\n", "visitTypeExpr");
    return NULL;
}

static TypecheckError* visitTypeDeclStmtNode(Typechecker* tc, Node* node) {
    TypeDeclStmt* typeDeclStmt = node->as.typeDeclStmt;
    printf("%s\n", "visitTypeDeclStmtNode");
    visitTypeExpr(tc, typeDeclStmt->typeExpr);
    return NULL;
}

static TypecheckError* visitArrayLiteralNode(Typechecker* tc, Node* node) {
    ArrayLiteralNode* arrayLiteralNode = node->as.arrayLiteralNode;
    printf("%s\n", "visitArrayLiteralNode");
    for (int i = 0; i < arrayLiteralNode->size; ++i) {
        visit(tc, arrayLiteralNode->elements[i]);
    }
    return NULL;
}

static TypecheckError* visitObjectLiteralNode(Typechecker* tc, Node* node) {
    ObjectLiteralNode* objectLiteralNode = node->as.objectLiteralNode;
    printf("%s\n", "visitObjectLiteralNode");
    for (int i = 0; i < objectLiteralNode->size; ++i) {
        visit(tc, objectLiteralNode->entries[i]->value);
    }
    return NULL;
}

static TypecheckError* visitGroupingNodeNode(Typechecker* tc, Node* node) {
    GroupingNode* groupingNode = node->as.groupingNode;
    visit(tc, groupingNode->expr);
    node->type = groupingNode->expr->type;
    return NULL;
}

// Must keep order with AST_NODE_TYPES enum
TypecheckRule rules[] = {
    {NODE_TYPE_LITERAL,             visitLiteralNode},
    {NODE_TYPE_ARRAY_LITERAL,       visitArrayLiteralNode},
    {NODE_TYPE_OBJECT_LITERAL,      visitObjectLiteralNode},
    {NODE_TYPE_IDENT,               visitIdentifierNode},
    {NODE_TYPE_UNARY,               visitUnaryNode},
    {NODE_TYPE_BINARY,              visitBinaryNode},
    {NODE_TYPE_GROUPING,            visitGroupingNodeNode},
    {NODE_TYPE_IF_ELSE,             visitIfElseNode},
    {NODE_TYPE_BLOCK,               visitBlockNode},
    {NODE_TYPE_INVOCATION,          visitInvocationNode},
    {NODE_TYPE_VAL_DECL_STATEMENT,  visitValDeclStmtNode},
    {NODE_TYPE_FUNC_DECL_STATEMENT, visitFuncDeclStmtNode},
    {NODE_TYPE_TYPE_DECL_STATEMENT, visitTypeDeclStmtNode}
};

static TypecheckError* visit(Typechecker* tc, Node* node) {
    TypecheckFn fn = rules[node->nodeType].fn;
    return fn(tc, node);
}

int typecheck(Typechecker* tc) {
    if (tc->nodes->count == 0) {
        fprintf(stdout, "No nodes, nothing to do!\n");
        return 0;
    }

    for (int i = 0; i < tc->nodes->count; ++i) {
        Node* node = (Node*) tc->nodes->values[i];
        TypecheckError* err = visit(tc, node);
        if (err != NULL) {
            listAdd(tc->errors, (void**) &err);
        }
    }

    return tc->errors->count;
}

void initTypechecker(Typechecker* tc);

Typechecker* newTypechecker(List* nodes) {
    Typechecker* tc = malloc(sizeof(Typechecker));
    tc->depthMap = newNodeDepthMap();
    tc->errors = newList();
    tc->nodes = nodes;
    tc->scopes = stack_new();

    initTypechecker(tc);

    return tc;
}

void initTypechecker(Typechecker* tc) {
    map_t scope = hashmap_new();
    stack_push(tc->scopes, &scope);
}

void beginScope(Typechecker* tc) {
    map_t scope = hashmap_new();
    stack_push(tc->scopes, &scope);
}

void endScope(Typechecker* tc) {
    map_t oldScope;
    stack_pop(tc->scopes, &oldScope);
    hashmap_free(oldScope);
}

TypecheckError* newTypecheckError(Token* token, Type actualType, int numExpected, ...) {
    va_list args;
    va_start(args, numExpected);

    TypecheckError* err = malloc(sizeof(TypecheckError));
    err->token = token;
    err->actualType = actualType;
    err->numExpected = numExpected;
    err->expectedTypes = malloc(sizeof(Type) * numExpected);

    for (int i = 0; i < numExpected; ++i) {
        err->expectedTypes[i] = va_arg(args, Type);
    }

    return err;
}
