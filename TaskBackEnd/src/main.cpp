#include <iostream>
#include <nlohmann/json.hpp>
#include "message.hpp"
#include "parser.hpp"
#include "json_schema.hpp"
int main() {
    using namespace Schema;

/* 
    JsonField age(JsonField::FieldType::Number);
    age.add_restriction(RangeOfValue<int>{0, 120});

    JsonField name(JsonField::FieldType::String);

    schema.set_field("age", age);
    schema.set_field("name", name);

    nlohmann::json js = {
        {"age", 25},
        {"name", "Matias"}
    };

    auto result = schema.validate(js);

    assert(result.success);

    for (const auto& report : result.report["validation_result"]) {
        assert(report["errors"].empty());
    }

    std::cout << "Test 1 OK\n"; 
*/
/*     
    JsonSchema schema;
    JsonField age(JsonField::FieldType::Number);
    age.add_restriction(RangeOfValue<int>{0,120});

    JsonField name(JsonField::FieldType::String);

    schema.set_field("age", age);
    schema.set_field("name", name);

    nlohmann::json js = {
        {"age", -5},
        {"name", 123}
    };

    auto result = schema.validate(js);

    assert(!result.success);

    auto reports = result.report["validation_result"];

    assert(!reports[0]["errors"].empty());
    assert(!reports[1]["errors"].empty());

    std::cout << "Test 2 OK\n"; 
    */
    JsonSchema schema;

    JsonField age(JsonField::FieldType::Number);
    age.add_restriction(RangeOfValue<int>{0,120});

    JsonField name(JsonField::FieldType::String);

    schema.set_field("age", age);
    schema.set_field("name", name);

    nlohmann::json js = {
        {"age", 150},
        {"name", "Matias"}
    };

    auto result = schema.validate(js);

    assert(!result.success);

    auto reports = result.report["validation_result"];

    int valid_fields = 0;
    int invalid_fields = 0;

    for (const auto& report : reports) {
        if (report["errors"].empty())
            ++valid_fields;
        else
            ++invalid_fields;
    }

    assert(valid_fields == 1);
    assert(invalid_fields == 1);

    std::cout << "Test 3 OK\n";
}
