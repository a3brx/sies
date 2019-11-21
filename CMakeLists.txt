cmake_minimum_required(VERSION 3.14)
project(Sies C)

set(CMAKE_C_STANDARD 99)

add_executable(Sies main.c chess.c board.c board.h)