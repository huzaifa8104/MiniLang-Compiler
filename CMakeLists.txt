cmake_minimum_required(VERSION 3.10)
project(MyCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

# Source files
set(SOURCES
    main.cpp
    lexer.cpp
    parser.cpp
)

add_executable(mycompiler ${SOURCES})
