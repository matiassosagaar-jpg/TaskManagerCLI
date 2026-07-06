#include "parser.hpp"
nlohmann::json Parser::parse_string(const std::string& str) {
    return nlohmann::json::parse(str);
}