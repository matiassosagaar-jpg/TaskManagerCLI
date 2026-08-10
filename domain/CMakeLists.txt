add_library(domain
    src/task.cpp
    src/section.cpp
    src/task_manager.cpp
)
target_link_libraries(domain
    PRIVATE
        json_schema
)
target_include_directories(domain PUBLIC
    include
)