cmake_minimum_required(VERSION 3.24)
project(Threads C)

set(CMAKE_C_STANDARD 11)

add_executable(Threads main.c)
add_executable(project project.c)
add_executable(matmult matmult.c)

