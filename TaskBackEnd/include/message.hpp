#pragma once
#include <string>

struct message {
    int id;
    std::string type;
    std::string body;
    message(int, std::string, std::string);
};
