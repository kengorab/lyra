#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parser/ast.h>

#include "to_json_visitor.h"
#include "parser/ast.h"

static FILE* file;

static void visit(Node* node);

static void visitType(Type* type);

static void visitLiteralNode(LiteralNode* literalNode) {
    fprintf(file, "\"literalKind\": \"%s\", \"value\":", literalNodeTypes[literalNode->type]);
    switch (literalNode->type) {
        case LITERAL_NODE_INT: {
            fprintf(file, "%d", literalNode->iVal);
            break;
        }
        case LITERAL_NODE_DOUBLE: {
            fprintf(file, "%f", literalNode->dVal);
            break;
        }
        case LITERAL_NODE_STRING: {
            // TODO: Don't store quotes in strVal!
            fprintf(file, "\"%.*s\"", literalNode->str.length - 2, literalNode->str.val + 1);
            break;
        }
        case LITERAL_NODE_BOOL: {
            fprintf(file, "%s", literalNode->bVal ? "true" : "false");
            break;
        }
    }
}

static void visitUnaryNode(UnaryNode* node) {
    Token* token = node->token;
    fprintf(file, "\"operator\": {\"value\": \"%s\", \"position\": [%d, %d]},", tokenGetValue(token), token->line,
            token->col);
    fprintf(file, "\"expr\": ");
    visit(node->expr);
}

static void visitBinaryNode(BinaryNode* node) {
    Token* token = node->token;
    fprintf(file, "\"operator\": {\"value\": \"%s\", \"position\": [%d, %d]},", tokenGetValue(token), token->line,
            token->col);
    fprintf(file, "\"left\": ");
    visit(node->lExpr);
    fprintf(file, ",\"right\": ");
    visit(node->rExpr);
}

static void visitIdentifierNode(IdentifierNode* identifierNode) {
    fprintf(file, "\"name\": \"%s\"", identifierNode->name);
}

static void visitIfElseNode(IfElseNode* ifElseNode) {
    fprintf(file, "\"condition\": ");
    visit(ifElseNode->conditionExpr);
    fprintf(file, ", \"then\": ");
    visit(ifElseNode->thenExpr);
    fprintf(file, ", \"else\": ");
    if (ifElseNode->elseExpr != NULL) {
        visit(ifElseNode->elseExpr);
    } else {
        fprintf(file, "null");
    }
}

static void visitBlockNode(BlockNode* blockNode) {
    fprintf(file, "\"children\": [");
    for (int i = 0; i < blockNode->numExprs; ++i) {
        visit(blockNode->exprs[i]);
        if (i != blockNode->numExprs - 1)
            fprintf(file, ",");
    }
    fprintf(file, "]");
}

static void visitInvocationNode(InvocationNode* node) {
    fprintf(file, "\"target\": ");
    visit(node->target);
    fprintf(file, ", \"arguments\": [");
    for (int i = 0; i < node->numArgs; ++i) {
        visit(node->arguments[i]);
        if (i != node->numArgs - 1)
            fprintf(file, ",");
    }

    fprintf(file, "]");
}

static void visitTypeExpr(TypeExpr* typeExpr);

static void visitValDeclStmtNode(ValDeclStmt* stmt) {
    fprintf(file, "\"isMutable\": %s,", stmt->isMutable ? "true" : "false");

    fprintf(file, "\"name\": \"%s\", ", stmt->ident->name);

    fprintf(file, "\"annotatedType\": ");
    if (stmt->typeAnnotation != NULL)
        visitTypeExpr(stmt->typeAnnotation);
    else
        fprintf(file, "null");

    fprintf(file, ", \"assignment\": ");
    visit(stmt->assignment);
}

static void visitFuncDeclStmtNode(FuncDeclStmt* stmt) {
    fprintf(file, "\"name\": \"%s\",", stmt->name->name);

    fprintf(file, "\"parameters\": [");
    for (int i = 0; i < stmt->numParams; ++i) {
        visit(stmt->params[i]);
        if (i != stmt->numParams - 1)
            fprintf(file, ",");
    }

    fprintf(file, "], \"annotatedParameterTypes\": [");
    for (int i = 0; i < stmt->numParams; ++i) {
        visitTypeExpr(stmt->paramTypeAnnotations[i]);
        if (i != stmt->numParams - 1)
            fprintf(file, ",");
    }

    fprintf(file, "], \"body\": ");
    visit(stmt->body);

    fprintf(file, ", \"annotatedReturnType\": ");
    if (stmt->returnTypeAnnotation != NULL)
        visitTypeExpr(stmt->returnTypeAnnotation);
    else
        fprintf(file, "null");
}

static void visitTypeExpr(TypeExpr* typeExpr) {
    fprintf(file, "{\"typeExprKind\": \"%s\",", typeExprTypes[typeExpr->type]);

    switch (typeExpr->type) {
        case TYPE_EXPR_STRUCT_TYPE: {
            fprintf(file, "\"fields\": [");

            for (int i = 0; i < typeExpr->as.structType.numFields; ++i) {
                fprintf(file, "{\"key\": ");
                visit(typeExpr->as.structType.keys[i]);
                fprintf(file, ", \"value\": ");
                visitTypeExpr(typeExpr->as.structType.fields[i]);
                fprintf(file, "}");

                if (i != typeExpr->as.structType.numFields)
                    fprintf(file, ", ");
            }

            fprintf(file, "]}");
            return;
        }
        case TYPE_EXPR_TUPLE_TYPE: {
            fprintf(file, "\"elementTypes\": [");

            for (int i = 0; i < typeExpr->numArgs; ++i) {
                visitTypeExpr(typeExpr->typeArgs[i]);
                if (i != typeExpr->numArgs - 1)
                    fprintf(file, ", ");
            }

            fprintf(file, "]}");
            return;
        }
        case TYPE_EXPR_BASIC_TYPE: {
            fprintf(file, "\"name\": \"%s\",", typeExpr->as.basicType.name->name);
            fprintf(file, "\"typeArgs\": [");
            for (int i = 0; i < typeExpr->numArgs; ++i) {
                visitTypeExpr(typeExpr->typeArgs[i]);
                if (i != typeExpr->numArgs - 1)
                    fprintf(file, ", ");
            }

            fprintf(file, "]}");
            return;
        }
        case TYPE_EXPR_ENUM_TYPE: {
            fprintf(file, "\"options\": [");

            for (int i = 0; i < typeExpr->as.enumType.numOptions; ++i) {
                visitTypeExpr(typeExpr->as.enumType.options[i]);
                if (1 != typeExpr->as.enumType.numOptions - 1) {
                    fprintf(file, ",");
                }
            }
            fprintf(file, "]}");
        }
        default: {
            // Invalid state
            return;
        }
    }
}

static void visitTypeDeclStmtNode(TypeDeclStmt* typeDeclStmt) {
    fprintf(file, "\"name\" : \"%s\",", typeDeclStmt->name->name);
    fprintf(file, "\"typeArgs\": [");
    for (int i = 0; i < typeDeclStmt->numArgs; ++i) {
        fprintf(file, "\"%s\"", typeDeclStmt->typeArgs[i]->name);
        if (i != typeDeclStmt->numArgs - 1)
            fprintf(file, ", ");
    }

    fprintf(file, "]");

    fprintf(file, ", \"typeExpr\": ");
    visitTypeExpr(typeDeclStmt->typeExpr);
}

static void visitArrayLiteralNode(ArrayLiteralNode* node) {
    fprintf(file, "\"elements\": [");
    for (int i = 0; i < node->size; ++i) {
        visit(node->elements[i]);
        if (i != node->size - 1)
            fprintf(file, ",");
    }

    fprintf(file, "]");
}

static void visitObjectLiteralNode(ObjectLiteralNode* node) {
    fprintf(file, "\"entries\": [");
    for (int i = 0; i < node->size; ++i) {
        fprintf(file, "{\"key\": ");
        visit(node->entries[i]->ident);
        fprintf(file, ", \"value\": ");
        visit(node->entries[i]->value);
        fprintf(file, "}");

        if (i != node->size - 1)
            fprintf(file, ",");
    }

    fprintf(file, "]");
}

static void visitGroupingNodeNode(GroupingNode* node) {
    fprintf(file, "\"expr\": ");
    visit(node->expr);
}

static void visitType(Type* type) {
    fprintf(file, "%s%s", type->name, type->numTypeArgs == 0 ? "" : "[");

    for (int i = 0; i < type->numTypeArgs; ++i) {
        visitType(type->typeArgs[i]);
        if (i < type->numTypeArgs - 1)
            fprintf(file, ", ");
    }

    fprintf(file, "%s", type->numTypeArgs == 0 ? "" : "]");
}

static void visit(Node* node) {
    Token* token = NODE_GET_TOKEN_HACK(node);
    fprintf(file, "{\"kind\": \"%s\", \"position\": [%d, %d],", astNodeTypes[node->nodeType], token->line, token->col);
    fprintf(file, "\"type\": \"");
    visitType(node->type);
    fprintf(file, "\",");

    switch (node->nodeType) {
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
    fprintf(file, "}");
}

void visitTypes(Typechecker* tc) {
    int numTypes;
    char** types = hashmap_keys(tc->typesMap, &numTypes);

    fprintf(file, "{");
    for (int i = 0; i < numTypes; ++i) {
        Type* type;
        get_TypesMap(tc->typesMap, types[i], &type);

        fprintf(
            file,
            "\"%s\": {"
            "\"type\": \"%s\","
            "\"name\": \"%s%s",
            types[i],
            primitiveTypes[type->type],
            type->name,
            type->numTypeArgs == 0 ? "" : "["
        );

        for (int j = 0; j < type->numTypeArgs; ++j) {
            fprintf(file, "%s", type->typeArgNames[j]);
            if (j < type->numTypeArgs - 1)
                fprintf(file, ", ");
        }
        fprintf(file, "%s\",", type->numTypeArgs == 0 ? "" : "]");

        fprintf(
            file,
            "\"parent\": \"%s\"",
            type->parent == NULL ? "Any" : type->parent->name
        );

        fprintf(file, "}%s", i == numTypes - 1 ? "" : ",");
    }
    fprintf(file, "}");
}


void toJson_visit(const char* filePath, Typechecker* tc) {
    file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error opening file for writing: %s\n", filePath);
        exit(1);
    }

    fprintf(file, "{\"types\": ");
    visitTypes(tc);
    fprintf(file, ",\"ast\": { \"nodes\": [");

    for (int i = 0; i < tc->nodes->count; ++i) {
        visit((Node*) tc->nodes->values[i]);
        if (i != tc->nodes->count - 1)
            fprintf(file, ",");
    }

    fprintf(file, "]}}");
    fclose(file);
}

