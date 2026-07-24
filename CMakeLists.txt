cmake_minimum_required(VERSION 3.20)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(domain)

add_executable(domain_tests
    tests/tests.cpp
)

target_link_libraries(domain_tests PRIVATE domain)