set(CMAKE_C_COMPILER "C:/ProgramData/chocolatey/bin/gcc.exe") # e.g., /usr/bin/gcc
set(CMAKE_CXX_COMPILER "C:/ProgramData/chocolatey/bin/g++.exe") # e.g., /usr/bin/g++

cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)
