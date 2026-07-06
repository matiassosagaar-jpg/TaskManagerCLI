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

std::unique_ptr<Message> from_json(const nlohmann::json& js) {
    switch (Message::parse_type(js["type"])) {
    case Message::Type::Message:
        return std::make_unique<Message>(
            js["id"].get<int>(),
            js["body"].get<std::string>()
        );
    case Message::Type::Reply:
        return Reply::from_json(js);
    case Message::Type::Error:
        return Error::from_json(js);
    }
    throw std::invalid_argument("Unkown type"); // this should never happen though
    // since it's supposed to get a validated json
}

nlohmann::json Message::to_json() const {
    nlohmann::json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["body"] = get_body();
    return js;
}
// -------------------Reply-------------------

Reply::Reply(int id, std::string body, int reply_to)
    : Message(id,body), reply_to {reply_to} {
}

Message::Type Reply::get_type() const {
    return Message::Type::Reply;
}

nlohmann::json Reply::to_json() const {
    nlohmann::json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["body"] = get_body();
    js["reply_to"] = reply_to;
    return js;
}
std::unique_ptr<Reply> Reply::from_json(const nlohmann::json& js) {
    return std::make_unique<Reply>(
        js["id"].get<int>(),
        js["body"].get<std::string>(),
        js["reply_to"].get<int>()
    );
}
// -------------------Error-------------------
Error::Error(int id, std::string error_code) 
: Message(id, error_code) {
}

Message::Type Error::get_type() const {
    return Message::Type::Error;
}

nlohmann::json Error::to_json() const {
    nlohmann::json js;
    js["id"] = get_id();
    js["type"] = type_to_str(get_type());
    js["error_code"] = get_body();
    return js;
}
std::unique_ptr<Error> Error::from_json(const nlohmann::json& js) {
    return std::make_unique<Error>(
        js["id"].get<int>(),
        js["error_code"].get<std::string>()
    );
}