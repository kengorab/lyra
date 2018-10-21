cmake_minimum_required(VERSION 3.9)
project(clyra C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(RUNTIME_OUTPUT_DIRECTORY build)

include_directories(src)
include_directories(test)

add_executable(
        clyra
        src/common/list.c
        src/common/list.h
        src/debugging/mem_checks.h
        src/debugging/printing_visitor.c
        src/debugging/printing_visitor.h
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.h
        src/parser/ast.c
        src/parser/parser.c
        src/parser/parser.h
        src/parser/parser_error.c
        src/parser/parser_error.h
        src/main.c
        src/common/strings.c
        src/common/strings.h
        src/typechecker/typechecker.c src/typechecker/typechecker.h)

enable_testing()
add_executable(
        clyra_tests
        src/common/list.c
        src/common/list.h
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.h
        src/parser/ast.c
        src/parser/parser.c
        src/parser/parser.h
        src/parser/parser_error.c
        src/parser/parser_error.h
        test/common/list_test.c
        test/common/list_test.h
        test/lexer/lexer_test.c
        test/lexer/lexer_test.h
        test/parser/ast_test.c
        test/parser/ast_test.h
        test/parser/array_tests.c
        test/parser/array_tests.h
        test/parser/block_tests.c
        test/parser/block_tests.h
        test/parser/func_decl_tests.c
        test/parser/func_decl_tests.h
        test/parser/ifelse_tests.c
        test/parser/ifelse_tests.h
        test/parser/invocation_tests.c
        test/parser/invocation_tests.h
        test/parser/parser_test.c
        test/parser/parser_test.h
        test/parser/type_decl_tests.c
        test/parser/type_decl_tests.h
        test/main.c
        test/test.h
        test/parser/literal_tests.c
        test/parser/literal_tests.h
        test/parser/object_tests.c
        test/parser/object_tests.h
        test/parser/unary_binary_tests.c
        test/parser/unary_binary_tests.h
        test/test_utils.c
        test/test_utils.h
        test/parser/val_decl_tests.c
        test/parser/val_decl_tests.h
        src/common/strings.c
        src/common/strings.h
)
add_test(clyra_tests clyra_tests)
