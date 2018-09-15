#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    printf(" = ");
    visit(stmt->assignment);
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

