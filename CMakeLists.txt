cmake_minimum_required(VERSION 3.21)
project(untitled C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.cpp)

add_executable(untitled lab1/zad2/main.c lab1/zad1/lib_counting.c lab1/zad1/lib_counting.h)
add_executable(lab2 lab2/zad1/main.c)