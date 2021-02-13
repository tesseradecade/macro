cmake_minimum_required(VERSION 3.17)
project(macro C)

set(CMAKE_C_STANDARD 99)

add_library(macro macro.c macro.h)