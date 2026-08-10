cmake_minimum_required(VERSION 3.20)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(domain)
add_subdirectory(persistence)

add_executable(domain_tests
    tests/domain_tests.cpp
)
add_executable(persistence_test
    tests/persistence_tests.cpp)

include(FetchContent)

FetchContent_Declare(
    JsonSchema
    GIT_REPOSITORY https://github.com/matiassosagaar-jpg/json-schema-cpp.git
    GIT_TAG 520e170
)
FetchContent_MakeAvailable(JsonSchema)

target_link_libraries(domain_tests 
    PRIVATE
        domain
        json_schema
)
target_link_libraries(persistence_test
    PRIVATE
        persistence
        domain
        json_schema
)