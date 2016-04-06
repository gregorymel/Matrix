cmake_minimum_required(VERSION 2.8)
 
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions ( -std=c++11 -O0)
 
add_executable(runTests test.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
