add_library(persistence
    src/saver.cpp
    src/report_schema.cpp
    src/persistence_helpers.cpp
    src/json_schema_adapter.cpp
)
target_link_libraries(persistence
    PRIVATE
        domain
)

target_link_libraries(persistence
    PRIVATE
        json_schema
)
target_include_directories(persistence PUBLIC
    include
)