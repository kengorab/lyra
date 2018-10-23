#include <stdio.h>
#include <parser/ast.h>

#include "typechecker.h"

static void visit(Node* node);

static void visitTypeExpr(TypeExpr* typeExpr);

static void visitLiteralNode(Node* node) {
    LiteralNode* literalNode = node->as.literalNode;
    printf("%s\n", "visitLiteralNode");
}

static void visitUnaryNode(Node* node) {
    UnaryNode* unaryNode = node->as.unaryNode;
    printf("%s\n", "visitUnaryNode");
    visit(unaryNode->expr);
}

static void visitBinaryNode(Node* node) {
    BinaryNode* binaryNode = node->as.binaryNode;
    printf("%s\n", "visitBinaryNode");
    visit(binaryNode->lExpr);
    visit(binaryNode->rExpr);
}

static void visitIdentifierNode(Node* node) {
    IdentifierNode* identifierNode = node->as.identifierNode;
    printf("%s\n", "visitIdentifierNode");
}

static void visitIfElseNode(Node* node) {
    IfElseNode* ifElseNode = node->as.ifElseNode;
    printf("%s\n", "visitIfElseNode");
    visit(ifElseNode->conditionExpr);
    visit(ifElseNode->thenExpr);
    if (ifElseNode->elseExpr != NULL) {
        visit(ifElseNode->elseExpr);
    }
}

static void visitBlockNode(Node* node) {
    BlockNode* blockNode = node->as.blockNode;
    printf("%s\n", "visitBlockNode");
    for (int i = 0; i < blockNode->numExprs; ++i) {
        visit(blockNode->exprs[i]);
    }
}

static void visitInvocationNode(Node* node) {
    InvocationNode* invocationNode = node->as.invocationNode;
    printf("%s\n", "visitInvocationNode");
    visit(invocationNode->target);
    for (int i = 0; i < invocationNode->numArgs; ++i) {
        visit(invocationNode->arguments[i]);
    }
}

static void visitValDeclStmtNode(Node* node) {
    ValDeclStmt* valDeclStmt = node->as.valDeclStmt;
    printf("%s\n", "visitValDeclStmtNode");
    visit(valDeclStmt->assignment);
}

static void visitFuncDeclStmtNode(Node* node) {
    FuncDeclStmt* funcDeclStmt = node->as.funcDeclStmt;
    printf("%s\n", "visitFuncDeclStmtNode");
    visit(funcDeclStmt->body);
}

static void visitTypeExpr(TypeExpr* typeExpr) {
    printf("%s\n", "visitTypeExpr");
}

static void visitTypeDeclStmtNode(Node* node) {
    TypeDeclStmt* typeDeclStmt = node->as.typeDeclStmt;
    printf("%s\n", "visitTypeDeclStmtNode");
}

static void visitArrayLiteralNode(Node* node) {
    ArrayLiteralNode* arrayLiteralNode = node->as.arrayLiteralNode;
    printf("%s\n", "visitArrayLiteralNode");
    for (int i = 0; i < arrayLiteralNode->size; ++i) {
        visit(arrayLiteralNode->elements[i]);
    }
}

static void visitObjectLiteralNode(Node* node) {
    ObjectLiteralNode* objectLiteralNode = node->as.objectLiteralNode;
    printf("%s\n", "visitObjectLiteralNode");
    for (int i = 0; i < objectLiteralNode->size; ++i) {
        visit(objectLiteralNode->entries[i]->value);
    }
}

static void visitGroupingNodeNode(Node* node) {
    GroupingNode* groupingNode = node->as.groupingNode;
    printf("%s\n", "visitGroupingNodeNode");
    visit(groupingNode->expr);
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

void visit(Node* node) {
    TypecheckFn fn = rules[node->nodeType].fn;
    fn(node);
}

void typecheck(List* nodes) {
    if (nodes->count == 0) {
        fprintf(stdout, "No nodes, nothing to do!\n");
        return;
    }

    for (int i = 0; i < nodes->count; ++i) {
        Node* node = (Node*) nodes->values[i];
        visit(node);
    }
}
