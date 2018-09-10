#ifndef CLYRA_PARSER_H
#define CLYRA_PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Token** start;
    Token** current;
} Parser;

Parser newParser(Token** tokens);

Node* parse(Parser* parser);

#endif //CLYRA_PARSER_H
