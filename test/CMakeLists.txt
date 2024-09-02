cmake_minimum_required(VERSION 3.25)
project(miniTest)

set(CMAKE_CXX_STANDARD 23)

# I don't know why but cmake said it was not present inside the structure
set(GOOGLETEST_VERSION 1.13.0)

set(LIB ../src/mini.h ../src/mini.cpp)
set(TEST objectConstructionTest.cpp sectionInsertionTest.cpp)

add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR, ./lib/googletest/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(libMiniTest ${LIB})

add_executable(miniTest miniTest.cpp ${TEST})
target_link_libraries(miniTest gtest gtest_main libMiniTest)
