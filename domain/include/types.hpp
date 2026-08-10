#pragma once

#include <chrono>
#include <optional>
#include <ranges>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <concepts>
namespace Domain
{
enum class Priority
{
    Low,
    Medium,
    High,
    Urgent
};
constexpr std::array array_all_priorities{
    Priority::Low,
    Priority::Medium,
    Priority::High,
    Priority::Urgent
};
using Date = std::chrono::year_month_day;
using OptionalDate = std::optional<Date>;

}
