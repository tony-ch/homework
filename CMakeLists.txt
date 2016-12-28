cmake_minimum_required(VERSION 3.6)
project(compile)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        err.c
        err.h
        global.h
        lex.c
        main.c
        syntax.c midcode.c objcode.c objcode.h opt.c opt.h)

add_executable(compile ${SOURCE_FILES})