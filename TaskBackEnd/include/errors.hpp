#pragma once
#include <stdexcept>
namespace Errors {
    class ValidationError : public std::exception {
    public:
        const char* what() const noexcept override {return internal_string.c_str();} //overriding virtual what(), assuring it'll not throw ecceptions and it won't modify class state
        ValidationError(const std::string& str) : internal_string{str} {};
    private:
        std::string internal_string;
    };
}