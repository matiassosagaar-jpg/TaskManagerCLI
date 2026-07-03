#include "message.hpp"
#include <stdexcept>
Message::Type Message::parse_type(const std::string& str) {
    if (str == "message") return Type::Message;
    if (str == "reply") return Type::Reply;
    if (str == "error") return Type::Error;

    throw std::invalid_argument("Unknown message type");
}
std::string Message::type_to_str(Type type) {
    switch (type) {
    case Type::Message:
        return "message";
    case Type::Reply:
        return "reply";
    case Type::Error:
        return "error";
    }
    throw std::invalid_argument("Invalid enum value");
}
Message::Message(int id, std::string type, std::string body) :
id{id}, type{parse_type(type)}, body{body} {}



