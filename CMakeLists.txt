cmake_minimum_required(VERSION 3.24)
project(DynamicArray)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ITMO-GDS-algorithms
        array.h
        tests.cpp
#        main.cpp
        )
target_link_libraries(ITMO-GDS-algorithms gtest_main)
add_test(NAME example_test COMMAND example)