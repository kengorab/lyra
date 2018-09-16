#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parser/ast.h>

#include "printing_visitor.h"
#include "parser/ast.h"

static void visit(Node* node);

static void visitLiteralNode(LiteralNode* literalNode) {
    switch (literalNode->type) {
        case LITERAL_NODE_INT: {
            printf("%d", literalNode->iVal);
            break;
        }
        case LITERAL_NODE_DOUBLE: {
            printf("%f", literalNode->dVal);
            break;
        }
        case LITERAL_NODE_STRING: {
            // TODO: Don't store quotes in strVal!
            printf("\"%.*s\"", literalNode->str.length - 2, literalNode->str.val + 1);
            break;
        }
        case LITERAL_NODE_BOOL: {
            printf("%s", literalNode->bVal ? "true" : "false");
            break;
        }
    }
}

static void visitUnaryNode(UnaryNode* node) {
    printf("%.*s", node->token->length, node->token->start);
    visit(node->expr);
}

static void visitBinaryNode(BinaryNode* node) {
    visit(node->lExpr);
    printf(" %.*s ", node->token->length, node->token->start);
    visit(node->rExpr);
}

static void visitIdentifierNode(IdentifierNode* identifierNode) {
    printf("%.*s", identifierNode->token->length, identifierNode->name);
}

static void visitValDeclStmtNode(ValDeclStmt* stmt) {
    printf("val ");
    visitIdentifierNode(stmt->ident);
    printf(" = ");
    visit(stmt->assignment);
}

static void visitArrayLiteralNode(ArrayLiteralNode* node) {
    printf("[");
    for (int i = 0; i < node->size - 1; ++i) {
        visit(node->elements[i]);
        printf(", ");
    }

    if (node->size >= 1) {
        visit(node->elements[node->size - 1]);
    }

    printf("]");
}

static void visitObjectLiteralNode(ObjectLiteralNode* node) {
    printf("{ ");
    for (int i = 0; i < node->size - 1; ++i) {
        visitIdentifierNode(node->entries[i]->ident->as.identifierNode);
        printf(": ");
        visit(node->entries[i]->value);
        printf(", ");
    }

    if (node->size >= 1) {
        visitIdentifierNode(node->entries[node->size - 1]->ident->as.identifierNode);
        printf(": ");
        visit(node->entries[node->size - 1]->value);
    }

    printf(" }");
}

static void visitGroupingNodeNode(GroupingNode* node) {
    printf("(");
    visit(node->expr);
    printf(")");
}

static void visit(Node* node) {
    switch (node->type) {
        case NODE_TYPE_IDENT: {
            visitIdentifierNode(node->as.identifierNode);
            break;
        }
        case NODE_TYPE_LITERAL: {
            visitLiteralNode(node->as.literalNode);
            break;
        }
        case NODE_TYPE_UNARY: {
            visitUnaryNode(node->as.unaryNode);
            break;
        }
        case NODE_TYPE_BINARY: {
            visitBinaryNode(node->as.binaryNode);
            break;
        }
        case NODE_TYPE_ARRAY_LITERAL: {
            visitArrayLiteralNode(node->as.arrayLiteralNode);
            break;
        }
        case NODE_TYPE_OBJECT_LITERAL: {
            visitObjectLiteralNode(node->as.objectLiteralNode);
            break;
        }
        case NODE_TYPE_GROUPING: {
            visitGroupingNodeNode(node->as.groupingNode);
            break;
        }
        case NODE_TYPE_VAL_DECL_STATEMENT: {
            visitValDeclStmtNode(node->as.valDeclStmt);
            break;
        }
    }
}

void printing_visit(List* nodes) {
    if (nodes->count == 0) {
        fprintf(stdout, "No nodes, nothing to do!\n");
        return;
    }

    for (int i = 0; i < nodes->count; ++i) {
        visit((Node*) nodes->values[i]);
        printf("\n");
    }
}

