add_subdirectory(../googletest googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests main_test.cpp)
target_link_libraries(runUnitTests gtest gtest_main)
