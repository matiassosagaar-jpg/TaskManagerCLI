#include "report_schema.hpp"

namespace Persistence {
using namespace Schema;
JsonSchema task_schema() {
    JsonField priority {JsonField::FieldType::String};
    priority.add_restriction(SetOfValues<std::string>(get_priority_names()));
    return {
        {"id", JsonField{JsonField::FieldType::Number}},
        {"name", JsonField{JsonField::FieldType::String}},
        {"description", JsonField{JsonField::FieldType::String}},
        {"priority", priority},
        {"section", JsonField{JsonField::FieldType::Number}},
        {"deadline", JsonField{JsonField::FieldType::String}}
    };
}

JsonSchema section_schema() {
    return {
        {"name", JsonField{JsonField::FieldType::String}},
        {"id" , JsonField{JsonField::FieldType::Number}},
        {"tasks", JsonField{JsonField::FieldType::Array}}
    };
}

JsonSchema manager_schema() {
    return {
        {"task_reports", JsonField{JsonField::FieldType::Array}},
        {"section_reports", JsonField{JsonField::FieldType::Array}},
        {"last_task_id", JsonField{JsonField::FieldType::Number}},
        {"last_section_id", JsonField{JsonField::FieldType::Number}}
    };
}
}// namespace Persistence