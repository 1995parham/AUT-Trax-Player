cmake_minimum_required(VERSION 3.1)
project(AUT_Trax_Player)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0 -g -DDEBUG")

set(SOURCE_FILES
    src/game.c
    src/main.c
    src/traxcore.c
    src/traxmove.c
    src/AI.c
    )

add_executable(AUT_Trax_Player ${SOURCE_FILES})