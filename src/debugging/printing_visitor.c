#include <stdio.h>

#include "printing_visitor.h"
#include "../parser/ast.h"

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
    printf("%s", identifierNode->name);
}

void printing_visit(Node* node) {
    switch (node->type) {
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
