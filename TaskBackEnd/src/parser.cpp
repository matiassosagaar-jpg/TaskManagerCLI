#include "parser.hpp"
nlohmann::json Parser::parse_string(const std::string& str) {
    using json = nlohmann::json;
    return json::parse(str);
}