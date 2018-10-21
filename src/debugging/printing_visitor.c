#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parser/ast.h>

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
            printf("\"%.*s\"", literalNode->str.length - 2, literalNode->str.val + 1);
            break;
        }
        case LITERAL_NODE_BOOL: {
            printf("%s", literalNode->bVal ? "true" : "false");
            break;
        }
    }
}

static void visitUnaryNode(UnaryNode* node) {
    printf("%.*s", node->token->length, node->token->start);
    visit(node->expr);
}

static void visitBinaryNode(BinaryNode* node) {
    visit(node->lExpr);
    printf(" %.*s ", node->token->length, node->token->start);
    visit(node->rExpr);
}

static void visitIdentifierNode(IdentifierNode* identifierNode) {
    printf("%s", identifierNode->name);
}

static void visitIfElseNode(IfElseNode* ifElseNode) {
    printf("if (");
    visit(ifElseNode->conditionExpr);
    printf(") ");
    visit(ifElseNode->thenExpr);
    if (ifElseNode->elseExpr != NULL) {
        printf(" else ");
        visit(ifElseNode->elseExpr);
    }
}

static void visitBlockNode(BlockNode* blockNode) {
    printf("{\n");
    for (int i = 0; i < blockNode->numExprs - 1; ++i) {
        printf("  ");
        visit(blockNode->exprs[i]);
        printf("\n");
    }

    if (blockNode->numExprs >= 1) {
        printf("  ");
        visit(blockNode->exprs[blockNode->numExprs - 1]);
        printf("\n");
    }
    printf("}");
}

static void visitInvocationNode(InvocationNode* node) {
    visit(node->target);
    printf("(");
    for (int i = 0; i < node->numArgs - 1; ++i) {
        if (strlen(node->argNames[i]) != 0) {
            printf("%s: ", node->argNames[i]);
        }
        visit(node->arguments[i]);
        printf(", ");
    }

    if (node->numArgs >= 1) {
        if (strlen(node->argNames[node->numArgs - 1]) != 0) {
            printf("%s: ", node->argNames[node->numArgs - 1]);
        }
        visit(node->arguments[node->numArgs - 1]);
    }
    printf(")");
}

static void visitTypeExpr(TypeExpr* typeExpr);

static void visitValDeclStmtNode(ValDeclStmt* stmt) {
    if (stmt->isMutable) {
        printf("var ");
    } else {
        printf("val ");
    }

    visitIdentifierNode(stmt->ident);
    if (stmt->typeAnnotation != NULL) {
        printf(": ");
        visitTypeExpr(stmt->typeAnnotation);
    }

    printf(" = ");
    visit(stmt->assignment);
}

static void visitFuncDeclStmtNode(FuncDeclStmt* stmt) {
    printf("func ");
    visitIdentifierNode(stmt->name);
    printf("(");
    for (int i = 0; i < stmt->numParams - 1; ++i) {
        visitIdentifierNode(stmt->params[i]->as.identifierNode);
        if (stmt->paramTypeAnnotations[i] != NULL) {
            printf(": ");
            visitTypeExpr(stmt->paramTypeAnnotations[i]);
        }
        printf(", ");
    }

    if (stmt->numParams >= 1) {
        visitIdentifierNode(stmt->params[stmt->numParams - 1]->as.identifierNode);
        if (stmt->paramTypeAnnotations[stmt->numParams - 1] != NULL) {
            printf(": ");
            visitTypeExpr(stmt->paramTypeAnnotations[stmt->numParams - 1]);
        }
    }
    printf(")");
    if (stmt->returnTypeAnnotation != NULL) {
        printf(": ");
        visitTypeExpr(stmt->returnTypeAnnotation);
    }
    printf(" ");

    if (stmt->body->type != NODE_TYPE_BLOCK) {
        printf("= ");
    }
    visit(stmt->body);
}

static void visitTypeExpr(TypeExpr* typeExpr) {
    switch (typeExpr->type) {
        case TYPE_EXPR_STRUCT_TYPE: {
            printf("{ ");

            for (int i = 0; i < typeExpr->as.structType.numFields - 1; ++i) {
                visit(typeExpr->as.structType.keys[i]);
                printf(": ");
                visitTypeExpr(typeExpr->as.structType.fields[i]);
                printf(", ");
            }

            if (typeExpr->as.structType.numFields >= 1) {
                visit(typeExpr->as.structType.keys[typeExpr->as.structType.numFields - 1]);
                printf(": ");
                visitTypeExpr(typeExpr->as.structType.fields[typeExpr->as.structType.numFields - 1]);
            }

            printf(" }");
            return;
        }
        case TYPE_EXPR_TUPLE_TYPE: {
            printf("[");
            for (int i = 0; i < typeExpr->numArgs - 1; ++i) {
                visitTypeExpr(typeExpr->typeArgs[i]);
                printf(", ");
            }

            if (typeExpr->numArgs >= 1) {
                visitTypeExpr(typeExpr->typeArgs[typeExpr->numArgs - 1]);
            }

            printf("]");
            return;
        }
        case TYPE_EXPR_BASIC_TYPE: {
            visitIdentifierNode(typeExpr->as.basicType.name);
            if (typeExpr->numArgs != 0) {
                printf("[");
                for (int i = 0; i < typeExpr->numArgs - 1; ++i) {
                    visitTypeExpr(typeExpr->typeArgs[i]);
                    printf(", ");
                }

                if (typeExpr->numArgs >= 1) {
                    visitTypeExpr(typeExpr->typeArgs[typeExpr->numArgs - 1]);
                }

                printf("]");
            }
            return;
        }
        case TYPE_EXPR_ENUM_TYPE: {
            for (int i = 0; i < typeExpr->as.enumType.numOptions - 1; ++i) {
                visitTypeExpr(typeExpr->as.enumType.options[i]);
                printf(" | ");
            }

            if (typeExpr->as.enumType.numOptions >= 1) {
                visitTypeExpr(typeExpr->as.enumType.options[typeExpr->as.enumType.numOptions - 1]);
            }
        }
        default: {
            // Invalid state
            return;
        }
    }
}

static void visitTypeDeclStmtNode(TypeDeclStmt* typeDeclStmt) {
    printf("type ");
    visitIdentifierNode(typeDeclStmt->name);
    if (typeDeclStmt->numArgs != 0) {
        printf("[");
        for (int i = 0; i < typeDeclStmt->numArgs - 1; ++i) {
            visitIdentifierNode(typeDeclStmt->typeArgs[i]);
            printf(", ");
        }

        if (typeDeclStmt->numArgs >= 1) {
            visitIdentifierNode(typeDeclStmt->typeArgs[typeDeclStmt->numArgs - 1]);
        }
        printf("]");
    }

    printf(" = ");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    visitTypeExpr(typeExpr);
}

static void visitArrayLiteralNode(ArrayLiteralNode* node) {
    printf("[");
    for (int i = 0; i < node->size - 1; ++i) {
        visit(node->elements[i]);
        printf(", ");
    }

    if (node->size >= 1) {
        visit(node->elements[node->size - 1]);
    }

    printf("]");
}

static void visitObjectLiteralNode(ObjectLiteralNode* node) {
    printf("{ ");
    for (int i = 0; i < node->size - 1; ++i) {
        visitIdentifierNode(node->entries[i]->ident->as.identifierNode);
        printf(": ");
        visit(node->entries[i]->value);
        printf(", ");
    }

    if (node->size >= 1) {
        visitIdentifierNode(node->entries[node->size - 1]->ident->as.identifierNode);
        printf(": ");
        visit(node->entries[node->size - 1]->value);
    }

    printf(" }");
}

static void visitGroupingNodeNode(GroupingNode* node) {
    printf("(");
    visit(node->expr);
    printf(")");
}

static void visit(Node* node) {
    switch (node->type) {
        case NODE_TYPE_IDENT: {
            visitIdentifierNode(node->as.identifierNode);
            break;
        }
        case NODE_TYPE_LITERAL: {
            visitLiteralNode(node->as.literalNode);
            break;
        }
        case NODE_TYPE_UNARY: {
            visitUnaryNode(node->as.unaryNode);
            break;
        }
        case NODE_TYPE_BINARY: {
            visitBinaryNode(node->as.binaryNode);
            break;
        }
        case NODE_TYPE_ARRAY_LITERAL: {
            visitArrayLiteralNode(node->as.arrayLiteralNode);
            break;
        }
        case NODE_TYPE_OBJECT_LITERAL: {
            visitObjectLiteralNode(node->as.objectLiteralNode);
            break;
        }
        case NODE_TYPE_GROUPING: {
            visitGroupingNodeNode(node->as.groupingNode);
            break;
        }
        case NODE_TYPE_VAL_DECL_STATEMENT: {
            visitValDeclStmtNode(node->as.valDeclStmt);
            break;
        }
        case NODE_TYPE_FUNC_DECL_STATEMENT: {
            visitFuncDeclStmtNode(node->as.funcDeclStmt);
            break;
        }
        case NODE_TYPE_TYPE_DECL_STATEMENT: {
            visitTypeDeclStmtNode(node->as.typeDeclStmt);
            break;
        }
        case NODE_TYPE_IF_ELSE: {
            visitIfElseNode(node->as.ifElseNode);
            break;
        }
        case NODE_TYPE_BLOCK: {
            visitBlockNode(node->as.blockNode);
            break;
        }
        case NODE_TYPE_INVOCATION: {
            visitInvocationNode(node->as.invocationNode);
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

