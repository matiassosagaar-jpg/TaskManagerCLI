#include "message.hpp"
#include <nlohmann/json.hpp>

namespace Parser {
    using json = nlohmann::json;
    json parse_string(const std::string&);
};