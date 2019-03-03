#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/ast.h"
#include "typechecker.h"
#include "common/stack.h"
#include "common/hashmap.h"

void beginScope(Typechecker* tc);

void endScope(Typechecker* tc);

static TypecheckError* define(Typechecker* tc, Token* token, const char* name, Type* type);

static TypecheckError* newTypeMismatchError(Token* token, Type* actualType, int numExpected, ...);

static TypecheckError* newCustomError(Token* token, const char* message);

static TypecheckError* visit(Typechecker* tc, Node* node);

static TypecheckError* visitTypeExpr(Typechecker* tc, TypeExpr* typeExpr, Type** outType);

static TypecheckError* visitLiteralNode(Typechecker* tc, Node* node) {
    LiteralNode* literalNode = node->as.literalNode;
    switch (literalNode->type) {
        case LITERAL_NODE_STRING: {
            node->type = typeString();
            break;
        }
        case LITERAL_NODE_INT: {
            node->type = typeInt();
            break;
        }
        case LITERAL_NODE_DOUBLE: {
            node->type = typeDouble();
            break;
        }
        case LITERAL_NODE_BOOL: {
            node->type = typeBool();
            break;
        }
        case LITERAL_NODE_NIL: {
            node->type = typeNil();
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
                return newTypeMismatchError(unaryNode->token, unaryNode->expr->type, 2, typeInt(), typeDouble());
            }
            node->type = unaryNode->expr->type;
            break;
        }
        case TOKEN_BANG: {
            if (!NODE_IS_BOOL(unaryNode->expr)) {
                return newTypeMismatchError(unaryNode->token, unaryNode->expr->type, 1, typeBool());
            }
            node->type = typeBool();
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
                    node->type = typeString();
                    return NULL;
                }
            }

            if (!NODE_IS_NUMERIC(binaryNode->lExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->lExpr->type, 2, typeInt(), typeDouble());
            } else if (!NODE_IS_NUMERIC(binaryNode->rExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->rExpr->type, 2, typeInt(), typeDouble());
            }

            if (binaryNode->token->type == TOKEN_SLASH) {
                node->type = typeDouble();
                return NULL;
            }

            if (NODE_IS_INT(binaryNode->lExpr) && NODE_IS_INT(binaryNode->rExpr)) {
                node->type = typeInt();
                return NULL;
            }

            node->type = typeDouble();
            return NULL;
        }
        case TOKEN_LT:
        case TOKEN_LTE:
        case TOKEN_GT:
        case TOKEN_GTE: {
            if (NODE_IS_NUMERIC(binaryNode->lExpr) && NODE_IS_NUMERIC(binaryNode->rExpr)) {
                node->type = typeBool();
                return NULL;
            } else if (!NODE_IS_NUMERIC(binaryNode->lExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->lExpr->type, 2, typeInt(), typeDouble());
            } else if (!NODE_IS_NUMERIC(binaryNode->rExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->rExpr->type, 2, typeInt(), typeDouble());
            }
        }
        case TOKEN_EQ_EQ:
        case TOKEN_BANG_EQ: {
            node->type = typeBool();
            return NULL;
        }
        case TOKEN_AND:
        case TOKEN_OR: {
            if (NODE_IS_BOOL(binaryNode->lExpr) && NODE_IS_BOOL(binaryNode->rExpr)) {
                node->type = typeBool();
                return NULL;
            } else if (!NODE_IS_BOOL(binaryNode->lExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->lExpr->type, 1, typeBool());
            } else if (!NODE_IS_BOOL(binaryNode->rExpr)) {
                return newTypeMismatchError(binaryNode->token, binaryNode->rExpr->type, 1, typeBool());
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
    const char* name = identifierNode->name;

    Type* type = NULL;
    int i = 0;
    do {
        map_t scope;
        if (stack_peek_n(tc->scopes, &scope, i++) != STACK_OK) {
            char msg[30 + strlen(name)];
            snprintf(msg, sizeof(msg), "Binding with name %s not found", name);
            return newCustomError(identifierNode->token, msg);
        }

        hashmap_get(scope, (char*) name, (void**) &type);
    } while (type == NULL);

    node->type = type;

    return NULL;
}

static TypecheckError* visitIfElseNode(Typechecker* tc, Node* node) {
    IfElseNode* ifElseNode = node->as.ifElseNode;
    visit(tc, ifElseNode->conditionExpr);
    if (!NODE_IS_BOOL(ifElseNode->conditionExpr)) {
        Token* token = NODE_GET_TOKEN_HACK(ifElseNode->conditionExpr);
        return newTypeMismatchError(token, ifElseNode->conditionExpr->type, 1, typeBool());
    }

    visit(tc, ifElseNode->thenExpr);
    if (ifElseNode->elseExpr != NULL) {
        visit(tc, ifElseNode->elseExpr);

        if (ifElseNode->thenExpr->type->type == ifElseNode->elseExpr->type->type) {
            node->type = ifElseNode->thenExpr->type;
        }
    }
    // TODO: If there is no else, the type of the Node should be Opt[THEN_TYPE]
    // TODO: If the then and else branches don't match, the type of the Node should be an Either[THEN_TYPE, ELSE_TYPE]
    return NULL;
}

static TypecheckError* visitBlockNode(Typechecker* tc, Node* node) {
    BlockNode* blockNode = node->as.blockNode;

    beginScope(tc);

    for (int i = 0; i < blockNode->numExprs; ++i) {
        TypecheckError* err = visit(tc, blockNode->exprs[i]);
        if (err != NULL) {
            return err;
        }

        if (i == blockNode->numExprs - 1) {
            node->type = blockNode->exprs[i]->type;
        }
    }

    endScope(tc);

    return NULL;
}

static TypecheckError* visitInvocationNode(Typechecker* tc, Node* node) {
    InvocationNode* invocationNode = node->as.invocationNode;
    visit(tc, invocationNode->target);
    Type* targetType = invocationNode->target->type;
    if (!NODE_IS_FUNCTION(invocationNode->target)) {
        const char* typeName = targetType->name;
        char msg[32 + strlen(typeName)];
        snprintf(msg, sizeof(msg), "Cannot invoke type %s as function", typeName);
        return newCustomError(invocationNode->token, msg);
    }

    int numExpectedParams = targetType->numTypeArgs - 1; // Subtract 1 to account for return type arg
    if (invocationNode->numArgs != numExpectedParams) {
        char msg[29];
        snprintf(msg, sizeof(msg), "Expected %d parameters, got %d", numExpectedParams, invocationNode->numArgs);
        return newCustomError(invocationNode->token, msg);
    }

    for (int i = 0; i < invocationNode->numArgs; ++i) {
        TypecheckError* err = visit(tc, invocationNode->arguments[i]);
        if (err != NULL) {
            return err;
        }

        const char* paramName = invocationNode->argNames[i];
        if (strlen(paramName) != 0) {
            const char* expectedArgName = targetType->typeArgNames[i + 1]; // Offset by 1 to account for _ret
            if (strcmp(expectedArgName, paramName) != 0) {
                char msg[52 + strlen(expectedArgName) + strlen(paramName)];
                snprintf(msg, sizeof(msg), "Incorrect name for named parameter: expected %s, got %s", expectedArgName,
                         paramName);
                return newCustomError(NODE_GET_TOKEN_HACK(invocationNode->arguments[i]), msg);
            }
        }

        Type* paramType = invocationNode->arguments[i]->type;
        if (!typeEq(paramType, targetType->typeArgs[i + 1])) {
            return newTypeMismatchError(NODE_GET_TOKEN_HACK(invocationNode->arguments[i]), paramType, 1,
                                        targetType->typeArgs[i + 1]);
        }
    }

    node->type = targetType->typeArgs[0];

    return NULL;
}

static TypecheckError* visitValDeclStmtNode(Typechecker* tc, Node* node) {
    node->type = typeUnit();
    ValDeclStmt* valDeclStmt = node->as.valDeclStmt;
    const char* name = valDeclStmt->ident->name;
    if (valDeclStmt->assignment == NULL) {
        if (valDeclStmt->isMutable) { // Assignment is not mandatory for `var` declarations
            if (valDeclStmt->typeAnnotation == NULL) {
                char _msg[55 + strlen(name)];
                snprintf(_msg, sizeof(_msg), "Missing required type annotation for mutable variable %s", name);
                return newCustomError(valDeclStmt->token, _msg);
            }

            Type* type = resolveType(valDeclStmt->typeAnnotation, tc->typesMap);
            TypecheckError* err = define(tc, valDeclStmt->token, name, type);
            if (err != NULL) return err;
        } else {
            char msg[52 + strlen(name)];
            snprintf(msg, sizeof(msg), "Missing required assignment for immutable variable %s", name);
            return newCustomError(valDeclStmt->token, msg);
        }
    } else {
        visit(tc, valDeclStmt->assignment);

        if (valDeclStmt->typeAnnotation == NULL) {
            TypecheckError* err = define(tc, valDeclStmt->token, name, valDeclStmt->assignment->type);
            if (err != NULL) return err;
        } else {
            Type* type = resolveType(valDeclStmt->typeAnnotation, tc->typesMap);
            TypecheckError* err = define(tc, valDeclStmt->token, name, type);
            if (err != NULL) return err;

            if (!typeEq(type, valDeclStmt->assignment->type)) {
                Token* token = valDeclStmt->assignment->as.literalNode->token;
                return newTypeMismatchError(token, valDeclStmt->assignment->type, 1, type);
            }
        }
    }

    return NULL;
}

static TypecheckError* visitFuncDeclStmtNode(Typechecker* tc, Node* node) {
    FuncDeclStmt* funcDeclStmt = node->as.funcDeclStmt;
    node->type = typeUnit();

    beginScope(tc);

    Type** paramTypes = calloc((size_t) funcDeclStmt->numParams, sizeof(Type*));
    TypecheckError* err;
    for (int i = 0; i < funcDeclStmt->numParams; i++) {
        paramTypes[i] = resolveType(funcDeclStmt->paramTypeAnnotations[i], tc->typesMap);
        Node* paramNode = funcDeclStmt->params[i];
        paramNode->type = paramTypes[i];
        IdentifierNode* param = paramNode->as.identifierNode;
        err = define(tc, param->token, param->name, paramTypes[i]);
        if (err != NULL) {
            return err;
        }
    }

    err = visit(tc, funcDeclStmt->body);
    endScope(tc); // TODO?: Assign scope onto FuncDeclNode to use later on?
    if (err != NULL) {
        return err;
    }

    Type* bodyType = funcDeclStmt->body->type;
    if (funcDeclStmt->returnTypeAnnotation == NULL) {
        const char** paramNames = calloc((size_t) funcDeclStmt->numParams, sizeof(char*));
        for (int i = 0; i < funcDeclStmt->numParams; ++i) {
            paramNames[i] = funcDeclStmt->params[i]->as.identifierNode->name;
        }

        Type* funcType = typeFunction(bodyType, funcDeclStmt->numParams, paramTypes, paramNames);
        free(paramTypes);
        free(paramNames);
        err = define(tc, funcDeclStmt->token, funcDeclStmt->name->name, funcType);
        if (err != NULL) {
            return err;
        }
    } else {
        Type* returnType = resolveType(funcDeclStmt->returnTypeAnnotation, tc->typesMap);
        if (!typeEq(bodyType, returnType)) {
            Token* token = NODE_GET_TOKEN_HACK(funcDeclStmt->body);
            return newTypeMismatchError(token, returnType, 1, bodyType);
        }
    }

    return NULL;
}

static TypecheckError* visitTypeExpr(Typechecker* tc, TypeExpr* typeExpr, Type** outType) {
    switch (typeExpr->type) {
        case TYPE_EXPR_BASIC_TYPE: {
            *outType = resolveType(typeExpr, tc->typesMap);
            break;
        }
        default:printf("Unknown type: %s; exiting\n", tokenGetValue(typeExpr->token));
            exit(1);
    }

    return NULL;
}

static TypecheckError* visitTypeDeclStmtNode(Typechecker* tc, Node* node) {
    TypeDeclStmt* typeDeclStmt = node->as.typeDeclStmt;
    node->type = typeUnit();

    if (tc->scopes->size != 1) {
        return newCustomError(typeDeclStmt->token, "Types cannot be declared outside of the top-level scope");
    }

    Type* type;
    TypecheckError* err = visitTypeExpr(tc, typeDeclStmt->typeExpr, &type);
    if (err != NULL) {
        return err;
    }

    const char* name = typeDeclStmt->name->name;
//    type = newTypeWithParent(name, type); // TODO: This might become necessary later?
    add_TypesMap(tc->typesMap, name, type);

    return NULL;
}

static TypecheckError* visitArrayLiteralNode(Typechecker* tc, Node* node) {
    ArrayLiteralNode* arrayLiteralNode = node->as.arrayLiteralNode;
    Type* t = NULL;
    for (int i = 0; i < arrayLiteralNode->size; ++i) {
        Node* item = arrayLiteralNode->elements[i];
        TypecheckError* err = visit(tc, item);
        if (err != NULL) {
            return err;
        }

        if (t == NULL) {
            t = item->type;
        } else if (!typeEq(t, item->type) && !typeEq(t, typeAny())) {
            t = typeAny(); // TODO: This should probably resolve to List[Type1 | Type2 | Type3] or something like that
        }
    }
    node->type = typeList(t);
    return NULL;
}

static TypecheckError* visitObjectLiteralNode(Typechecker* tc, Node* node) {
    ObjectLiteralNode* objectLiteralNode = node->as.objectLiteralNode;

    Type** fieldTypes = calloc((size_t) objectLiteralNode->size, sizeof(Type*));
    const char** fieldNames = calloc((size_t) objectLiteralNode->size, sizeof(char*));
    for (int i = 0; i < objectLiteralNode->size; ++i) {
        TypecheckError* err = visit(tc, objectLiteralNode->entries[i]->value);
        if (err != NULL) {
            return err;
        }
        fieldTypes[i] = objectLiteralNode->entries[i]->value->type;
        fieldNames[i] = objectLiteralNode->entries[i]->ident->as.identifierNode->name;

        objectLiteralNode->entries[i]->ident->type = fieldTypes[i];
    }

    node->type = typeObj(NULL, objectLiteralNode->size, fieldTypes, fieldNames);
    free(fieldTypes);
    free(fieldNames);

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
    tc->depthMap = newNodeDepthMap(); // Map<Node, int>
    tc->errors = newList(); // List<TypecheckError>
    tc->nodes = nodes; // List<Node>
    tc->scopes = stack_new(); // Stack<Map<String, Type>>
    tc->typesMap = newTypesMap(); // Map<String, Type>

    initTypechecker(tc);

    return tc;
}

void initTypechecker(Typechecker* tc) {
    beginScope(tc); // Begin global scope (scope at depth 0 is global)

    initTypesMap(tc->typesMap);
}

void beginScope(Typechecker* tc) {
    map_t scope = hashmap_new(); // Map<String, Type>
    stack_push(tc->scopes, &scope);
}

void endScope(Typechecker* tc) {
    map_t oldScope;
    stack_pop(tc->scopes, &oldScope);
    hashmap_free(oldScope);
}

static TypecheckError* define(Typechecker* tc, Token* token, const char* name, Type* type) {
    if (stack_is_empty(tc->scopes)) {
        printf("ERROR! Somehow the scope stack is empty");
        exit(1);
    }

    map_t scope;
    if (stack_peek(tc->scopes, &scope) != STACK_OK) {
        printf("ERROR! Somehow the scope stack is empty");
        exit(1);
    }

    void* _throwaway;
    if (hashmap_get(scope, name, &_throwaway) == MAP_OK) {
        char msg[46 + strlen(name)];
        snprintf(msg, sizeof(msg), "Variable %s cannot be re-declared in this scope", name);
        return newCustomError(token, msg);
    }

    hashmap_put(scope, name, type);
    return NULL;
}

static TypecheckError* newCustomError(Token* token, const char* message) {
    TypecheckError* err = malloc(sizeof(TypecheckError));
    err->kind = TYPE_ERROR_CUSTOM;
    err->custom.token = token;
    err->custom.message = message;

    return err;
}

static TypecheckError* newTypeMismatchError(Token* token, Type* actualType, int numExpected, ...) {
    va_list args;
    va_start(args, numExpected);

    TypecheckError* err = malloc(sizeof(TypecheckError));
    err->kind = TYPE_ERROR_MISMATCH;
    err->mismatch.token = token;
    err->mismatch.actualType = actualType;
    err->mismatch.numExpected = numExpected;
    err->mismatch.expectedTypes = malloc(sizeof(Type*) * numExpected);

    for (int i = 0; i < numExpected; ++i) {
        err->mismatch.expectedTypes[i] = va_arg(args, Type*);
    }

    return err;
}
