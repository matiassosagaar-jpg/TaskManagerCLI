#include "validator.hpp"
#include <map>
std::optional<Errors::ValidationError> Validator::validate(const json& js) {
    if (!js.contains("type")) {
        return Errors::ValidationError("No type field");
    }
    if (!js["type"].is_string()) {
        return Errors::ValidationError("\"type\" field si not a string");
    }
    if (!Validator::is_valid_field((js["type"].get<std::string>()),
            {"message", "reply", "error"}
        )
    ) {
        return Errors::ValidationError("Unknown type field");
    }
    if (!js.contains("body")){
        return Errors::ValidationError("No body field");
    }
    if (!js["body"].is_string()) {
        return Errors::ValidationError("\"body\" field si not a string");
    }
    if (js["type"] == "reply") {
        if (!js.contains("reply_to")){
            return Errors::ValidationError("No reply_to field for a reply type");
        }
        if (!js["reply_to"].is_string()){
            return Errors::ValidationError("\"reply_to\" field si not a string");
        }
    }
    return std::nullopt;
}
