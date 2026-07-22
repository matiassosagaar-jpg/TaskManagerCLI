cmake_minimum_required(VERSION 3.16)

project(TaskBackend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(backend src/main.cpp src/message.cpp)

target_include_directories(backend PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
