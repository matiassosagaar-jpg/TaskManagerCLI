#pragma once
#include "errors.hpp"
#include <optional>
#include <nlohmann/json.hpp>
namespace Validator {
    using json = nlohmann::json;
    std::optional<Errors::ValidationError> validate(const json&);
    template <typename T>
    bool is_valid_field(const T&, const std::vector<T>&);
}

template <typename T>
bool Validator::is_valid_field(const T& test_field, const std::vector<T>& valid_fields) {
    for (const T& valid_field : valid_fields) {
        if (test_field == valid_field) {
            return true;
        }
    }
    return false;
}