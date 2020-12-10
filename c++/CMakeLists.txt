cmake_minimum_required(VERSION 3.18)
project(AdventOfCode2020 LANGUAGES CXX)

find_package(fmt 7.1 REQUIRED)
find_package(Microsoft.GSL 3.1 REQUIRED)

# Require C++17 and disable any extensions for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

# Day 1
add_executable(day-01 day-01.cpp)
target_link_libraries(day-01 PRIVATE Microsoft.GSL::Microsoft.GSL fmt::fmt)
add_test(NAME day-01.test COMMAND day-01 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-01.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 692916\n.* 289270976\n")

# Day 2
add_executable(day-02 day-02.cpp)
target_link_libraries(day-02 PRIVATE fmt::fmt)
add_test(NAME day-02.test COMMAND day-02 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-02.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 580\n.* 611\n")

# Day 3
add_executable(day-03 day-03.cpp)
target_link_libraries(day-03 PRIVATE fmt::fmt)
add_test(NAME day-03.test COMMAND day-03 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-03.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 189\n.* 1718180100\n")

# Day 4
add_executable(day-04 day-04.cpp)
target_link_libraries(day-04 PRIVATE fmt::fmt)
add_test(NAME day-04.test COMMAND day-04 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-04.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 260\n.* 153\n")

# Day 5
add_executable(day-05 day-05.cpp)
target_link_libraries(day-05 PRIVATE fmt::fmt)
add_test(NAME day-05.test COMMAND day-05 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-05.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 848\n.* 682\n")

# Day 6
add_executable(day-06 day-06.cpp)
target_link_libraries(day-06 PRIVATE fmt::fmt)
add_test(NAME day-06.test COMMAND day-06 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-06.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 6703\n.* 3430\n")

# Day 7
add_executable(day-07 day-07.cpp)
target_link_libraries(day-07 PRIVATE fmt::fmt)
add_test(NAME day-07.test COMMAND day-07 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-07.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 335\n.* 2431\n")

# Day 8
add_executable(day-08 day-08.cpp)
target_link_libraries(day-08 PRIVATE fmt::fmt)
add_test(NAME day-08.test COMMAND day-08 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-08.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 1594\n.* 758\n")

# Day 9
add_executable(day-09 day-09.cpp)
target_link_libraries(day-09 PRIVATE Microsoft.GSL::Microsoft.GSL fmt::fmt)
add_test(NAME day-09.test COMMAND day-09 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(day-09.test PROPERTIES
    PASS_REGULAR_EXPRESSION " 1492208709\n.* 238243506\n")
