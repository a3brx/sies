cmake_minimum_required(VERSION 3.14)
project(sies C)

set(CMAKE_C_STANDARD 99)

add_executable(sies main.c chess/utils.c chess/chess.c)