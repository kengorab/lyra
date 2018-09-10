cmake_minimum_required(VERSION 3.9)
project(clyra C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(RUNTIME_OUTPUT_DIRECTORY build)

include_directories(src/lexer)
include_directories(test)
include_directories(test/lexer)

add_executable(
        clyra
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/main.c
        src/debugging/mem_checks.h
)

enable_testing()
add_executable(
        clyra_tests
        src/lexer/lexer.c
        src/lexer/lexer.h
        test/lexer/lexer_test.c
        test/lexer/lexer_test.h
        test/main.c
        test/test.h
)
add_test(clyra_tests clyra_tests)
