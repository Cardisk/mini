cmake_minimum_required(VERSION 3.25)
project(mini)

set(CMAKE_CXX_STANDARD 23)

set(LIBS mini.h mini.cpp)

add_executable(mini main.cpp ${LIBS})
