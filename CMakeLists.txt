cmake_minimum_required(VERSION 3.1)
project(AUT_Trax_Player)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/game.c
    src/game.h
    src/main.c
    src/traxcore.c
    src/traxcore.h
    )

add_executable(AUT_Trax_Player ${SOURCE_FILES} src/traxmove.h src/traxmove.c)