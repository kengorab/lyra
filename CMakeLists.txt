cmake_minimum_required(VERSION 3.9)
project(clyra C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(RUNTIME_OUTPUT_DIRECTORY build)

include_directories(src/lexer)
include_directories(src/parser)
include_directories(src/debugging)
include_directories(test)
include_directories(test/lexer)

add_executable(
        clyra
        src/debugging/mem_checks.h
        src/debugging/printing_visitor.c
        src/debugging/printing_visitor.h
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.h
        src/parser/ast.c
        src/parser/parser.c
        src/parser/parser.h
        src/main.c
        src/common/list.c src/common/list.h)

enable_testing()
add_executable(
        clyra_tests
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.h
        src/parser/ast.c
        src/parser/parser.c
        src/parser/parser.h
        test/lexer/lexer_test.c
        test/lexer/lexer_test.h
        test/main.c
        test/test.h
)
add_test(clyra_tests clyra_tests)
