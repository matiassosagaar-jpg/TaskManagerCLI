add_library(file_management
    src/file_handler.cpp
)

target_include_directories(file_management
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)