cmake_minimum_required(VERSION 3.16)
project(nano)

set(CMAKE_CXX_STANDARD 17)
add_executable(nano src/main.cpp src/Lexer.cpp src/Lexer.h src/AST.h src/Stack.cpp src/Stack.h src/Runner.cpp src/Runner.h)