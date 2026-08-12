cmake_minimum_required(VERSION 3.20)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(domain)
add_subdirectory(persistence/serialization)
add_subdirectory(persistence/file_management)

add_executable(domain_tests
    tests/domain_tests.cpp
)

add_executable(serialization_test
    tests/serialization_test.cpp
)

add_executable(file_manager_test
    tests/file_manager_test.cpp
)

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
)

target_link_libraries(serialization_test
    PRIVATE
        serialization
)

target_link_libraries(file_manager_test
    PRIVATE
        file_management
)