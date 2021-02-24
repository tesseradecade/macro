cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(macro
        DESCRIPTION "Micro parser written in pure C"
        LANGUAGES C)

set(MACRO_VERSION "1.0.0")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(MACRO_BUILD_TESTS "Enable tests building for macro")
set(headers "src/macro.h")

add_subdirectory(src)

if (MACRO_BUILD_TESTS)
    add_subdirectory(tests)
endif (MACRO_BUILD_TESTS)