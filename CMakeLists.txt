cmake_minimum_required(VERSION 3.15)
project(Sies C)

set(CMAKE_C_STANDARD 99)

add_executable(Sies main.c game/piece.c game/board.c)