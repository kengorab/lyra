#include <stdio.h>
#include <stdlib.h>
#include <parser/ast.h>

#include "parser/ast.h"
#include "compiler.h"
#include "vm/vm.h"

static void visit(Compiler* c, Node* node);

static void visitLiteralNode(Compiler* c, Node* node) {
    if (!NODE_IS_PRIMITIVE(node)) {
        printf("Unsupported literal type: %s\n", node->type->name);
        exit(1);
    }

    LiteralNode* literalNode = node->as.literalNode;
    Token* tok = literalNode->token;

    int constantIdx;
    switch (node->type->type) {
        case PRIMITIVE_TYPE_INT: {
            int val = literalNode->iVal;
            constantIdx = addConstant(c->currentChunk, val);
            break;
        }
        case PRIMITIVE_TYPE_DOUBLE: {
            double val = literalNode->dVal;
            constantIdx = addConstant(c->currentChunk, val);
            break;
        }
        default: {
            printf("Unsupported primitive type: %s\n", primitiveTypes[node->type->type]);
            exit(1);
        }
    }

    writeChunk(c->currentChunk, OP_CONSTANT, tok->line);
    writeChunk(c->currentChunk, constantIdx, tok->line);
}

static void visitUnaryNode(Compiler* c, Node* node) {
    UnaryNode* unaryNode = node->as.unaryNode;
    visit(c, unaryNode->expr);

    Token* tok = NODE_GET_TOKEN_HACK(node);

    switch (unaryNode->token->type) {
        case TOKEN_MINUS: {
            writeChunk(c->currentChunk, OP_NEGATE, tok->line);
            return;
        }
        default: {
            printf("Unsupported unary operation: %s\n", tokenTypes[unaryNode->token->type]);
            exit(1);
        }
    }
}

static void visitBinaryNode(Compiler* c, Node* node) {
    BinaryNode* binaryNode = node->as.binaryNode;
    visit(c, binaryNode->lExpr);
    visit(c, binaryNode->rExpr);

    Token* tok = binaryNode->token;

    switch (binaryNode->token->type) {
        case TOKEN_PLUS: {
            writeChunk(c->currentChunk, OP_ADD, tok->line);
            return;
        }
        case TOKEN_MINUS: {
            writeChunk(c->currentChunk, OP_SUB, tok->line);
            return;
        }
        case TOKEN_STAR: {
            writeChunk(c->currentChunk, OP_MUL, tok->line);
            return;
        }
        case TOKEN_SLASH: {
            writeChunk(c->currentChunk, OP_DIV, tok->line);
            return;
        }
        default: {
            printf("Unsupported binary operation: %s\n", tokenTypes[binaryNode->token->type]);
            exit(1);
        }
    }
}

static void visitGroupingNode(Compiler* c, Node* node) {
    GroupingNode* groupingNode = node->as.groupingNode;
    visit(c, groupingNode->expr);
}

// Must keep order with AST_NODE_TYPES enum
static CompilerRule rules[] = {
    {NODE_TYPE_LITERAL,  visitLiteralNode},
    {NODE_TYPE_ARRAY_LITERAL,       NULL},
    {NODE_TYPE_OBJECT_LITERAL,      NULL},
    {NODE_TYPE_IDENT,               NULL},
    {NODE_TYPE_UNARY,    visitUnaryNode},
    {NODE_TYPE_BINARY,   visitBinaryNode},
    {NODE_TYPE_GROUPING, visitGroupingNode},
    {NODE_TYPE_IF_ELSE,             NULL},
    {NODE_TYPE_BLOCK,               NULL},
    {NODE_TYPE_INVOCATION,          NULL},
    {NODE_TYPE_VAL_DECL_STATEMENT,  NULL},
    {NODE_TYPE_FUNC_DECL_STATEMENT, NULL},
    {NODE_TYPE_TYPE_DECL_STATEMENT, NULL}
};

static void visit(Compiler* c, Node* node) {
    CompilerFn fn = rules[node->nodeType].fn;
    fn(c, node);
}

Chunk compile(Typechecker* tc) {
    Chunk initialChunk;
    initChunk(&initialChunk);

    Compiler compiler = {.typechecker = tc, .currentChunk = &initialChunk};

    if (tc->nodes->count == 0) {
        fprintf(stdout, "No nodes, nothing to do!\n");
        return initialChunk;
    }

    int lastLineNum = 0;
    for (int i = 0; i < tc->nodes->count; ++i) {
        Node* node = (Node*) tc->nodes->values[i];
        visit(&compiler, node);

        Token* tok = NODE_GET_TOKEN_HACK(node);
        lastLineNum = tok->line;
    }

    // Ensure there's a return at the end, since the VM will loop until it reaches a return instr
    writeChunk(&initialChunk, OP_RETURN, lastLineNum + 1);
    return initialChunk;
}
