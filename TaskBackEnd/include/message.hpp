#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
class Message {
public:
    enum class Type {
        Message,
        Reply,
        Error
    };
    static Type parse_type(const std::string&);
    static std::string type_to_str(Type);
    // constructor
    Message(int, std::string, std::string);
    static std::unique_ptr<Message> from_json(const nlohmann::json&);
private:
    int id;
    Type type;
    std::string body;
};

class Reply : public Message {
    private:
    int reply_to; // simply the index of the message being replied to
};

class Error : public Message {
};