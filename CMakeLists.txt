cmake_minimum_required(VERSION 3.16)

project(Timur VERSION 0.1.0 LANGUAGES CXX)

find_package(BISON REQUIRED)

set(BISON_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.ypp)
set(BISON_OUTPUT_CPP ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.tab.cpp)
set(BISON_OUTPUT_H   ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.tab.hpp)

BISON_TARGET(parser ${BISON_INPUT_FILE} ${BISON_OUTPUT_CPP} DEFINES_FILE ${BISON_OUTPUT_H})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SOURCES src/main.cpp
    src/lexer.cpp
    src/seman.cpp
    ${BISON_parser_OUTPUTS})
add_executable(timur
    ${SOURCES}
)
target_compile_options(timur PRIVATE -ggdb)
