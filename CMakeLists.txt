cmake_minimum_required(VERSION 3.10)
project(interval_map)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.tpp")
