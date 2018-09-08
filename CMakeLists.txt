cmake_minimum_required(VERSION 3.12)
project(clyra C)

set(CMAKE_C_STANDARD 99)

include_directories(src/lexer)

add_executable(clyra src/main.c src/lexer/lexer.c src/lexer/lexer.h)