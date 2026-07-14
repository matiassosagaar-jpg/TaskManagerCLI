#include <iostream>
#include <nlohmann/json.hpp>
#include "message.hpp"
#include "parser.hpp"
#include "json_schema.hpp"
int main() {
    using json = nlohmann::json;
    using namespace Schema;
    JsonField field(JsonField::FieldType::Number);

    field.add_restriction(RangeOfValue<int>{0,100});
    field.add_restriction(SetOfValues<int>{10,20,30});
    //auto result = field.validate(150);
    //auto result = field.validate(50);
    auto result = field.validate(-5);

    std::cout << result.report << std::endl;
}