#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "parser_error.h"

const char* prettyTokenNames[] = { // These rules NEED to stay in Token order
    "number",      // TOKEN_NUMBER
    "+",           // TOKEN_PLUS
    "-",           // TOKEN_MINUS
    "*",           // TOKEN_STAR
    "/",           // TOKEN_SLASH
    "<",           // TOKEN_LT
    "<=",          // TOKEN_LTE
    ">",           // TOKEN_GT
    ">=",          // TOKEN_GTE
    "=",           // TOKEN_EQ
    "==",          // TOKEN_EQ_EQ
    "!",           // TOKEN_BANG
    "!=",          // TOKEN_BANG_EQ
    "&&",          // TOKEN_AND
    "||",          // TOKEN_OR
    "[",           // TOKEN_LBRACK
    "]",           // TOKEN_RBRACK
    "{",           // TOKEN_LBRACE
    "}",           // TOKEN_RBRACE
    "(",           // TOKEN_LPAREN
    ")",           // TOKEN_RPAREN
    ".",           // TOKEN_DOT
    "comma",       // TOKEN_COMMA
    ":",           // TOKEN_COLON
    "::",          // TOKEN_COLON_COLON
    "=>",          // TOKEN_ARROW
    "|",           // TOKEN_PIPE
    "string",      // TOKEN_STRING
    "identifier",  // TOKEN_IDENT
    "val",         // TOKEN_VAL
    "var",         // TOKEN_VAR
    "type",        // TOKEN_TYPE
    "func",        // TOKEN_FUNC
    "if",          // TOKEN_IF
    "else",        // TOKEN_ELSE
    "true",        // TOKEN_TRUE
    "false",       // TOKEN_FALSE
    "nil",         // TOKEN_NIL
    "",            // TOKEN_ERROR
    "end of file", // TOKEN_EOF
};

ParseError* newParseError(Token* actual, int numExpected, ...) {
    va_list args;
    va_start(args, numExpected);
    ParseError* e = malloc(sizeof(ParseError));
    e->actual = actual;
    e->numExpected = numExpected;
    if (numExpected == 0) {
        const char* text = va_arg(args, char*);
        e->expectedStr = text;
    } else {
        e->expected = malloc(sizeof(TokenType) * numExpected);

        for (int i = 0; i < numExpected; ++i) {
            TokenType type = va_arg(args, TokenType);
            e->expected[i] = type;
        }
    }

    va_end(args);
    return e;
}

static const char* getCaretLine(int col, int offset) {
    char* str = malloc((size_t) col + offset);
    for (int i = 0; i < col + offset - 1; ++i) {
        str[i] = ' ';
    }
    str[col + offset - 1] = '^';
    return str;
}

static const char* getErrorMessage(ParseError* error) {
    const char* actual = prettyTokenNames[error->actual->type];

    if (error->numExpected <= 1) {
        const char* expected;
        if (error->numExpected == 0) {
            expected = error->expectedStr;
        } else {
            expected = prettyTokenNames[error->expected[0]];
        }

        char* msg = malloc(strlen(expected) + strlen(actual) + 15);
        sprintf(msg, "Expected %s, saw %s", expected, actual);
        return msg;
    } else if (error->numExpected == 2) {
        const char* expected1 = prettyTokenNames[error->expected[0]];
        const char* expected2 = prettyTokenNames[error->expected[1]];
        char* msg = malloc(strlen(expected1) + strlen(expected2) + strlen(actual) + 19);
        sprintf(msg, "Expected %s or %s, saw %s", expected1, expected2, actual);
        return msg;
    } else {
        // TODO: Possibly handle more than 2 expected?
        return "";
    }
}

const char* parseErrorGetMessage(char* source, ParseError* parseError) {
    int lineNum = 1;
    int idx = 0;
    int start = -1;
    int end = 0;

    char c = source[idx];
    while (c != 0) {
        if (lineNum == parseError->actual->line && start == -1) {
            start = idx;
        }

        if (lineNum == parseError->actual->line + 1) {
            end = idx;
            break;
        }

        if (c == '\n') {
            lineNum++;
        }
        c = source[++idx];
    }

    const char* caret = getCaretLine(parseError->actual->col, 4);
    const char* errMsg = getErrorMessage(parseError);

    char* output = malloc(strlen(caret) + strlen(errMsg) + 20);
    sprintf(
        output,
        "Line %d, col %d: %s\n    %.*s\n%s",
        parseError->actual->line,
        parseError->actual->col,
        errMsg,
        end - start - 1,
        source + start,
        caret
    );
    return output;
}
