add_library(serialization
    src/saver.cpp
    src/report_schema.cpp
    src/persistence_helpers.cpp
    src/json_schema_adapter.cpp
)

target_link_libraries(serialization
    PUBLIC
        domain
        json_schema
)

target_include_directories(serialization PUBLIC
    include
)