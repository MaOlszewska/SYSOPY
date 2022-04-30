cmake_minimum_required(VERSION 3.21)
project(untitled C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.cpp)

add_executable(untitled lab6/zad2/client.c lab6/zad2/common.h lab6/zad2/server.c)
