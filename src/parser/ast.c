#include <stdlib.h>
#include <string.h>

#include "parser/ast.h"
#include "ast.h"

const char* astNodeTypes[] = {AST_NODE_TYPES};

const char* literalNodeTypes[] = {LITERAL_NODE_TYPES};

Node* newValDeclStmtNode(Token* token, Node* identNode, Node* assignment) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_VAL_DECL_STATEMENT;

    ValDeclStmt* valDeclStmt = malloc(sizeof(ValDeclStmt));
    valDeclStmt->token = token;
    valDeclStmt->assignment = assignment;
    valDeclStmt->ident = identNode->as.identifierNode;

    n->as.valDeclStmt = valDeclStmt;
    return n;
}

Node* newLiteralNode(Token* token) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_LITERAL;

    LiteralNode* literalNode = malloc(sizeof(LiteralNode));
    literalNode->token = token;
    switch (token->type) {
        case TOKEN_NUMBER: {
            bool hasDecimal = false;
            for (int i = 0; i < token->length; ++i) {
                if (token->start[i] == '.')
                    hasDecimal = true;
            }

            if (hasDecimal) {
                literalNode->type = LITERAL_NODE_DOUBLE;
                literalNode->dVal = strtod(token->start, NULL);
            } else {
                literalNode->type = LITERAL_NODE_INT;
                literalNode->iVal = (int) strtol(token->start, NULL, 10);
            }
            break;
        }
        case TOKEN_STRING: {
            literalNode->type = LITERAL_NODE_STRING;
            literalNode->str.val = token->start;
            literalNode->str.length = token->length;
            break;
        }
        case TOKEN_TRUE:
        case TOKEN_FALSE: {
            literalNode->type = LITERAL_NODE_BOOL;
            literalNode->bVal = token->type == TOKEN_TRUE;
            break;
        }
        case TOKEN_NIL: {
            literalNode->type = LITERAL_NODE_NIL;
            break;
        }
        default: {
            return NULL;
        }
    }

    n->as.literalNode = literalNode;
    return n;
}

Node* newIdentifierNode(Token* token) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_IDENT;

    IdentifierNode* ident = malloc(sizeof(IdentifierNode));
    ident->token = token;
    ident->name = token->start;

    n->as.identifierNode = ident;
    return n;
}
