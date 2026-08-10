#pragma once
#include <concepts>
#include "json_schema.hpp"
#include "types.hpp"
#include <string_view>
#include <variant>
#include "types.hpp"
#include <nlohmann/json.hpp>
#include "ids.hpp"
#include <ranges>
namespace Persistence {
constexpr std::array<std::pair<Domain::Priority, std::string_view>, 4> priority_map {{
    {Domain::Priority::Low,    "LOW"},
    {Domain::Priority::Medium, "MEDIUM"},
    {Domain::Priority::High,   "HIGH"},
    {Domain::Priority::Urgent, "URGENT"}
}};

std::string_view to_str(Domain::Priority priority);
Domain::Priority to_prio(const std::string_view& priority);
std::vector<std::string> get_priority_names();

template <Domain::ID id>
std::vector<uint64_t> get_values(const std::vector<id>& ids) {
    std::vector<uint64_t> out;
    out.reserve(ids.size());
    std::ranges::transform(
        ids,
        std::back_inserter(out),
        [](const id& x) {return x.value;}
    ); // pushback all the values wrapped by ids element into out
    return out;
}

}