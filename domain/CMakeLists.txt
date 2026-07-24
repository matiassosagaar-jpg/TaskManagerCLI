add_library(domain
    src/task.cpp
    src/section.cpp
    src/task_manager.cpp
)

target_include_directories(domain PUBLIC
    include
)