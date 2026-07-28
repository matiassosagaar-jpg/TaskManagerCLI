#pragma once

#include <chrono>
#include <optional>

namespace Domain
{

enum class Priority
{
    Low,
    Medium,
    High,
    Urgent
};

using Date = std::chrono::year_month_day;
using OptionalDate = std::optional<Date>;

}