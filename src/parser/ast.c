#include <stdlib.h>
#include <string.h>

#include "parser/ast.h"
#include "ast.h"

const char* astNodeTypes[] = {AST_NODE_TYPES};

const char* literalNodeTypes[] = {LITERAL_NODE_TYPES};

const char* typeExprTypes[] = {TYPE_EXPR_TYPES};

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
    ident->name = tokenGetValue(token);

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

Node* newInvocationNode(Token* token, Node* target, int numArgs, Node** arguments, char** argNames) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_INVOCATION;

    InvocationNode* invocation = malloc(sizeof(InvocationNode));
    invocation->token = token;
    invocation->target = target;
    invocation->numArgs = numArgs;
    invocation->arguments = arguments;
    invocation->argNames = argNames;

    n->as.invocationNode = invocation;
    return n;
}

// ------------------------------------
//             Type Exprs
// ------------------------------------

TypeExpr* newBasicTypeExpr(Token* token, IdentifierNode* name, int numArgs, TypeExpr** typeArgs) {
    TypeExpr* expr = malloc(sizeof(TypeExpr));
    expr->token = token;
    expr->type = TYPE_BASIC_TYPE;
    expr->numArgs = numArgs;
    expr->typeArgs = typeArgs;

    expr->as.basicType.name = name;

    return expr;
}

TypeExpr* newTupleTypeExpr(Token* token, int numArgs, TypeExpr** typeArgs) {
    TypeExpr* expr = malloc(sizeof(TypeExpr));
    expr->token = token;
    expr->type = TYPE_TUPLE_TYPE;
    expr->numArgs = numArgs;
    expr->typeArgs = typeArgs;

    return expr;
}

TypeExpr* newStructTypeExpr(Token* token, int numFields, Node** keys, TypeExpr** fields) {
    TypeExpr* expr = malloc(sizeof(TypeExpr));
    expr->token = token;
    expr->type = TYPE_STRUCT_TYPE;

    expr->as.structType.numFields = numFields;
    expr->as.structType.keys = keys;
    expr->as.structType.fields = fields;

    return expr;
}

TypeExpr* newEnumTypeExpr(Token* token, int numOptions, TypeExpr** options) {
    TypeExpr* expr = malloc(sizeof(TypeExpr));
    expr->token = token;
    expr->type = TYPE_ENUM_TYPE;

    expr->as.enumType.numOptions = numOptions;
    expr->as.enumType.options = options;

    return expr;
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

Node* newFuncDeclStmtNode(Token* token, Node* nameNode, int numParams, Node** params, Node* body) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_FUNC_DECL_STATEMENT;

    FuncDeclStmt* funcDeclStmt = malloc(sizeof(FuncDeclStmt));
    funcDeclStmt->token = token;
    funcDeclStmt->name = nameNode->as.identifierNode;
    funcDeclStmt->numParams = numParams;
    funcDeclStmt->params = params;
    funcDeclStmt->body = body;

    n->as.funcDeclStmt = funcDeclStmt;
    return n;
}

Node* newTypeDeclStmtNode(Token* token, IdentifierNode* name, TypeExpr* typeExpr, int numArgs, IdentifierNode** typeArgs) {
    Node* n = malloc(sizeof(Node));
    n->type = NODE_TYPE_TYPE_DECL_STATEMENT;

    TypeDeclStmt* typeDeclStmt = malloc(sizeof(TypeDeclStmt));
    typeDeclStmt->token = token;
    typeDeclStmt->name = name;
    typeDeclStmt->typeExpr = typeExpr;
    typeDeclStmt->numArgs = numArgs;
    typeDeclStmt->typeArgs = typeArgs;

    n->as.typeDeclStmt = typeDeclStmt;
    return n;
}
