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
            printf("\"%.*s\"", literalNode->token->length, literalNode->strVal);
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
            printf("%d\n", node->as.literalNode->token->length);
            visitLiteralNode(node->as.literalNode);
            break;
        }
    }
}
