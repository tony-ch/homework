﻿cmake_minimum_required(VERSION 3.8)
project(logic C)

set(CMAKE_C_STANDARD 99)

add_executable(logic main.c common.h util.c lex.c error.c syntax.c syntax.h midcode.c run.c)