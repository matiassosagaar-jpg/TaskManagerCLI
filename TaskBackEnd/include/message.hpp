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
    Message(int, std::string);
    static std::unique_ptr<Message> from_json(const nlohmann::json&);
    virtual Type get_type() const;
    virtual nlohmann::json to_json() const;
    // getter
    int get_id() const {return id;}
    std::string get_body() const {return body;}
private:
    int id;
    std::string body;
};

class Reply : public Message {
public:
    Reply(int, std::string, int);
    Message::Type get_type() const override;
    nlohmann::json to_json() const override;
    static std::unique_ptr<Reply> from_json(const nlohmann::json&);
private:
    int reply_to; // simply the index of the message being replied to
};

class Error : public Message {
    public:
    Error(int, std::string);
    Message::Type get_type() const override;
    nlohmann::json to_json() const override;
    static std::unique_ptr<Error> from_json(const nlohmann::json&);
};