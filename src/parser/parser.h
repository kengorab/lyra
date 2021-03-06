#ifndef CLYRA_PARSER_H
#define CLYRA_PARSER_H

#include "lexer/lexer.h"
#include "parser/ast.h"
#include "common/list.h"
#include "parser_error.h"

typedef struct {
    Token** start;
    Token** current;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,     // =
    PREC_OR,             // ||
    PREC_AND,            // &&
    PREC_EQUALITY,       // == !=
    PREC_COMPARISON,     // < > <= >=
    PREC_ADDITION,       // + -
    PREC_MULTIPLICATION, // * /
    PREC_UNARY,          // ! - +
    PREC_CALL,           // . () []
    PREC_PRIMARY
} Precedence;

typedef Node* (* PrefixFn)(Parser*, Token**, ParseError**);
typedef Node* (* InfixFn)(Parser*, Token**, Node**, ParseError**);

typedef struct {
    PrefixFn prefixFn;
    InfixFn infixFn;
    Precedence precedence;
} ParseRule;

Parser newParser(Token** tokens);

List* parse(Parser* parser, List** outErrList);

#endif //CLYRA_PARSER_H
