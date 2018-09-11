#include <stdio.h>
#include <ast.h>
#include <string.h>
#include <stdlib.h>

#include "printing_visitor.h"
#include "../parser/ast.h"

static void visitLiteralNode(LiteralNode* literalNode) {
    switch (literalNode->type) {
        case LITERAL_NODE_INT: {
            printf("%d", literalNode->iVal);
        }
        case LITERAL_NODE_DOUBLE: {
            printf("%f", literalNode->dVal);
            break;
        }
        case LITERAL_NODE_STRING: {
            // TODO: Don't store quotes in strVal!
            printf("\"%.*s\"", literalNode->token->length - 2, literalNode->strVal + 1);
            break;
        }
        case LITERAL_NODE_BOOL: {
            printf("%s", literalNode->bVal ? "true" : "false");
            break;
        }
    }
}

static void visitIdentifierNode(IdentifierNode* identifierNode) {
    printf("%.*s", identifierNode->token->length, identifierNode->name);
}

static void visitValDeclStmtNode(ValDeclStmt* stmt) {
    printf("val ");
    visitIdentifierNode(stmt->ident);
    printf(" = <null>");
}

static void visit(Node* node) {
    switch (node->type) {
        case NODE_TYPE_VAL_DECL_STATEMENT: {
            visitValDeclStmtNode(node->as.valDeclStmt);
            break;
        }
        case NODE_TYPE_IDENT: {
            visitIdentifierNode(node->as.identifierNode);
            break;
        }
        case NODE_TYPE_LITERAL: {
            visitLiteralNode(node->as.literalNode);
            break;
        }
    }
}

void printing_visit(Node* node) {
    if (node == NULL) {
        fprintf(stdout, "Node is NULL, cannot print!\n");
        return;
    }

    visit(node);
}

