#ifndef CLYRA_PARSER_ERROR_H
#define CLYRA_PARSER_ERROR_H

#include "lexer/lexer.h"

typedef struct {
    Token* actual;
    int numExpected;
    TokenType* expected;
    const char* expectedStr;
} ParseError;

ParseError* newParseError(Token* actual, int numExpected, ...);

const char* parseErrorGetMessage(char* source, ParseError* parseError);

#endif //CLYRA_PARSER_ERROR_H
