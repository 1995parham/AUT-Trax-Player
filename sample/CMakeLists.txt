# In The Name Of God
# ========================================
# [] File Name : CMakeLists.txt
#
# [] Creation Date : 27-02-2016
#
# [] Created By : Parham Alvani (parham.alvani@gmail.com)
# =======================================
add_executable(sample.out main.c
	game.c game.c
	board.c board.h
	)

# Add personal c flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -g -std=gnu11 ")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG")
