#ifndef CLYRA_UTILS_H
#define CLYRA_UTILS_H

#include "test.h"
#include "lexer/lexer.h"
#include "parser/ast.h"

Token* makeToken(const char* contents, TokenType type);

TestResult assertIdentNode(const char* testName, Node* n, const char* name);

TestResult assertLiteralNode(const char* testName, Node* n, LiteralNodeType litType, ...);

#endif //CLYRA_UTILS_H
