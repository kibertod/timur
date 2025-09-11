cmake_minimum_required(VERSION 3.16)

project(Timur VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SOURCES src/main.cpp
    src/lexer.cpp)
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
