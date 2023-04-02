cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

PROJECT(code-time-cli)

ADD_EXECUTABLE(${PROJECT_NAME} code-time-cli.cpp)
