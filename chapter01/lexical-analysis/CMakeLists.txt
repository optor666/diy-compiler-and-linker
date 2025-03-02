cmake_minimum_required(VERSION 3.30)
project(lexical-analysis C)

set(CMAKE_C_STANDARD 11)

add_executable(scc dynarray.c dynstring.c error.c lex.c scc.c)
