cmake_minimum_required(VERSION 3.15)
project(Sies C)

set(CMAKE_C_STANDARD 99)

add_executable(Sies main.c old/piece.c old/board.c old/chess.c)