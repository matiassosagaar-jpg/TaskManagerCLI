#include "json_schema_adapter.hpp"
#include "report_schema.hpp"
#include <iostream>
#include <assert.h>
int main() {
    using namespace Persistence;
    using namespace Domain::State;
    using json = nlohmann::json;
    // get_json from user
    std::string input{
        std::istreambuf_iterator<char>{std::cin}, // Beginning of stdin
        std::istreambuf_iterator<char>{} // default constructed stands for EOF
    };
    auto original = json::parse(input);
    //std::cout << original.dump(4) << std::endl;
    // match against a full_report schema
    Schema::JsonSchema schema {manager_schema()};   
    Schema::ValidationResult result {schema.validate(original)};
    if (!result.success) {
        std::cout << result.report.dump(4)<< std::endl;
        return 1;
    }
    // deserialize to a ManagerFullReport
    ManagerFullReport report = Adapter::full_report_from_json(original);
    // Turn back into a json
    json turned_back_to_js = Adapter::to_json(report);

    //compare both jsons (received-produced)
    if (turned_back_to_js != original) {
    std::cerr << "Round-trip failed\n";
    std::cerr << "Original:\n" << original.dump(4) << '\n';
    std::cerr << "Reconstructed:\n" << turned_back_to_js.dump(4) << '\n';
    return 1;
    }

    std::cout << "Round-trip successful!\n";
    return 0;
}