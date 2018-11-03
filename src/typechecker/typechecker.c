#include <stdio.h>
#include <stdlib.h>

#include "parser/ast.h"
#include "typechecker.h"

static TypecheckError* visit(Node* node);

static TypecheckError* visitTypeExpr(TypeExpr* typeExpr);

static TypecheckError* visitLiteralNode(Node* node) {
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

static TypecheckError* visitUnaryNode(Node* node) {
    UnaryNode* unaryNode = node->as.unaryNode;
    TypecheckError* err = visit(unaryNode->expr);
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

static TypecheckError* visitBinaryNode(Node* node) {
    BinaryNode* binaryNode = node->as.binaryNode;
    TypecheckError* err = visit(binaryNode->lExpr);
    if (err != NULL) return err;

    err = visit(binaryNode->rExpr);
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

static TypecheckError* visitIdentifierNode(Node* node) {
    IdentifierNode* identifierNode = node->as.identifierNode;
    return NULL;
}

static TypecheckError* visitIfElseNode(Node* node) {
    IfElseNode* ifElseNode = node->as.ifElseNode;
    printf("%s\n", "visitIfElseNode");
    visit(ifElseNode->conditionExpr);
    visit(ifElseNode->thenExpr);
    if (ifElseNode->elseExpr != NULL) {
        visit(ifElseNode->elseExpr);
    }
    return NULL;
}

static TypecheckError* visitBlockNode(Node* node) {
    BlockNode* blockNode = node->as.blockNode;
    printf("%s\n", "visitBlockNode");
    for (int i = 0; i < blockNode->numExprs; ++i) {
        visit(blockNode->exprs[i]);
    }
    return NULL;
}

static TypecheckError* visitInvocationNode(Node* node) {
    InvocationNode* invocationNode = node->as.invocationNode;
    printf("%s\n", "visitInvocationNode");
    visit(invocationNode->target);
    for (int i = 0; i < invocationNode->numArgs; ++i) {
        visit(invocationNode->arguments[i]);
    }
    return NULL;
}

static TypecheckError* visitValDeclStmtNode(Node* node) {
    ValDeclStmt* valDeclStmt = node->as.valDeclStmt;
    printf("%s\n", "visitValDeclStmtNode");
    visit(valDeclStmt->assignment);
    return NULL;
}

static TypecheckError* visitFuncDeclStmtNode(Node* node) {
    FuncDeclStmt* funcDeclStmt = node->as.funcDeclStmt;
    printf("%s\n", "visitFuncDeclStmtNode");
    visit(funcDeclStmt->body);
    return NULL;
}

static TypecheckError* visitTypeExpr(TypeExpr* typeExpr) {
    printf("%s\n", "visitTypeExpr");
    return NULL;
}

static TypecheckError* visitTypeDeclStmtNode(Node* node) {
    TypeDeclStmt* typeDeclStmt = node->as.typeDeclStmt;
    printf("%s\n", "visitTypeDeclStmtNode");
    visitTypeExpr(typeDeclStmt->typeExpr);
    return NULL;
}

static TypecheckError* visitArrayLiteralNode(Node* node) {
    ArrayLiteralNode* arrayLiteralNode = node->as.arrayLiteralNode;
    printf("%s\n", "visitArrayLiteralNode");
    for (int i = 0; i < arrayLiteralNode->size; ++i) {
        visit(arrayLiteralNode->elements[i]);
    }
    return NULL;
}

static TypecheckError* visitObjectLiteralNode(Node* node) {
    ObjectLiteralNode* objectLiteralNode = node->as.objectLiteralNode;
    printf("%s\n", "visitObjectLiteralNode");
    for (int i = 0; i < objectLiteralNode->size; ++i) {
        visit(objectLiteralNode->entries[i]->value);
    }
    return NULL;
}

static TypecheckError* visitGroupingNodeNode(Node* node) {
    GroupingNode* groupingNode = node->as.groupingNode;
    visit(groupingNode->expr);
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

static TypecheckError* visit(Node* node) {
    TypecheckFn fn = rules[node->nodeType].fn;
    return fn(node);
}

List* typecheck(List* nodes) {
    List* errors = newList();
    if (nodes->count == 0) {
        fprintf(stdout, "No nodes, nothing to do!\n");
        return errors;
    }

    for (int i = 0; i < nodes->count; ++i) {
        Node* node = (Node*) nodes->values[i];
        TypecheckError* err = visit(node);
        if (err != NULL) {
            listAdd(errors, (void**) &err);
        }
    }

    return errors;
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
