cmake_minimum_required(VERSION 3.6)
project(compile)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
        err.cpp
        global.h main.cpp
        lex.cpp
        syntax.cpp syntax.h
        midcode.cpp
        objcode.cpp objcode.h
        opt.cpp opt.h
        )

add_executable(compile ${SOURCE_FILES})