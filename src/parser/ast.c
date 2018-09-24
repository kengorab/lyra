#include <stdlib.h>
#include <string.h>

#include "parser/ast.h"
#include "ast.h"

const char* astNodeTypes[] = {AST_NODE_TYPES};

const char* literalNodeTypes[] = {LITERAL_NODE_TYPES};

// ------------------------------------
//             Expressions
// ------------------------------------

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

Node* newUnaryNode(Token* token, Node* expr) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_UNARY;

    UnaryNode* unary = malloc(sizeof(UnaryNode));
    unary->token = token;
    unary->expr = expr;

    n->as.unaryNode = unary;
    return n;
}

Node* newBinaryNode(Token* token, Node* lExpr, Node* rExpr) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_BINARY;

    BinaryNode* binary = malloc(sizeof(BinaryNode));
    binary->token = token;
    binary->lExpr = lExpr;
    binary->rExpr = rExpr;

    n->as.binaryNode = binary;
    return n;
}

Node* newArrayLiteralNode(Token* token, Node** elements, int size) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_ARRAY_LITERAL;

    ArrayLiteralNode* array = malloc(sizeof(ArrayLiteralNode));
    array->token = token;
    array->elements = elements;
    array->size = size;

    n->as.arrayLiteralNode = array;
    return n;
}

ObjectLiteralEntry* newObjectLiteralEntry(Node* key, Node* value) {
    ObjectLiteralEntry* entry = malloc(sizeof(ObjectLiteralEntry));
    entry->ident = key;
    entry->value = value;
    return entry;
}

Node* newObjectLiteralNode(Token* token, ObjectLiteralEntry** entries, Node** keys, int size) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_OBJECT_LITERAL;

    ObjectLiteralNode* object = malloc(sizeof(ObjectLiteralNode));
    object->token = token;
    object->size = size;
    object->entries = entries;
    object->keys = keys;

    n->as.objectLiteralNode = object;
    return n;
}

Node* newGroupingNode(Token* token, Node* expr) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_GROUPING;

    GroupingNode* grouping = malloc(sizeof(GroupingNode));
    grouping->token = token;
    grouping->expr = expr;

    n->as.groupingNode = grouping;
    return n;
}

Node* newIfElseNode(Token* token, Node* condExpr, Node* thenExpr, Node* elseExpr) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_IF_ELSE;

    IfElseNode* ifElse = malloc(sizeof(IfElseNode));
    ifElse->token = token;
    ifElse->conditionExpr = condExpr;
    ifElse->thenExpr = thenExpr;
    ifElse->elseExpr = elseExpr;

    n->as.ifElseNode = ifElse;
    return n;
}

Node* newBlockNode(Token* token, Node** exprs, int numExprs) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_BLOCK;

    BlockNode* block = malloc(sizeof(BlockNode));
    block->token = token;
    block->exprs = exprs;
    block->numExprs = numExprs;

    n->as.blockNode = block;
    return n;
}

// ------------------------------------
//             Statements
// ------------------------------------

Node* newValDeclStmtNode(Token* token, Node* identNode, Node* assignment, bool isMutable) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_VAL_DECL_STATEMENT;

    ValDeclStmt* valDeclStmt = malloc(sizeof(ValDeclStmt));
    valDeclStmt->token = token;
    valDeclStmt->assignment = assignment;
    valDeclStmt->ident = identNode->as.identifierNode;
    valDeclStmt->isMutable = isMutable;

    n->as.valDeclStmt = valDeclStmt;
    return n;
}

