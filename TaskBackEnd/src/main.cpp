#include <iostream>
#include <nlohmann/json.hpp>
#include "message.hpp"
#include "parser.hpp"
#include "validator.hpp"
int main() {
    using json = nlohmann::json;
    std::string input;
    std::getline(std::cin, input);
    
    //parsing
    json js;
    try {
        js = Parser::parse_string(input);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    //validating
    auto report = Validator::validate(js);

    if (report) {
        std::cerr << report.value().what() << std::endl;
    }
}