#include "message.hpp"
#include <stdexcept>
// -------------------Message-------------------
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

Message::Message(int id, std::string body) :
id{id}, body{body} {}

Message::Type Message::get_type() const {
    return Message::Type::Message;
}

static std::unique_ptr<Message> from_json(const nlohmann::json& js) {
    using Type = Message::Type;
    Type type = Message::parse_type(js["type"]);
    switch (type) {
    case Type::Message :
    case Type::Error :
        return std::make_unique<Message>(
            js["id"].get<int>(),
            js["body"].get<std::string>()
        );
    case Type::Reply:
        return std::make_unique<Message>(
            js["id"].get<int>(),
            js["body"].get<std::string>(),
            js["reply_to"].get<int>()
        );
    }
}

nlohmann::json Message::to_json() const {
    using json = nlohmann::json;
    json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["body"] = get_body();
    return js;
}
// -------------------Reply-------------------

Reply::Reply(int id, Message::Type type, std::string body, int reply_to)
    : Message(id,body), reply_to {reply_to} {
}

Message::Type Reply::get_type() const {
    return Message::Type::Reply;
}

nlohmann::json Reply::to_json() const {
    using json = nlohmann::json;
    json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["body"] = get_body();
    js["reply_to"] = reply_to;
    return js;
}
// -------------------Error-------------------

Message::Type Error::get_type() const {
    return Message::Type::Error;
}

nlohmann::json Error::to_json() const {
    using json = nlohmann::json;
    json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["error_code"] = get_body();
    return js;
}