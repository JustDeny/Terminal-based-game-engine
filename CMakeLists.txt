cmake_minimum_required(VERSION 3.22)
project(ConsoleGameEngine)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS ${GCC_COVERAGE_FLAGS})
find_package(Curses REQUIRED)
add_executable(ConsoleGameEngine main.cpp tbGameEngine.cpp tbGameEngine.h)
target_link_libraries(ConsoleGameEngine ${CURSES_LIBRARIES})