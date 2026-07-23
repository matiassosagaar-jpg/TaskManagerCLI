#pragma once
#include <cstdint>
#include <compare>
namespace Domain {
struct TaskID {
    uint64_t value {};

    explicit TaskID(uint64_t v) : value(v) {}

    auto operator<=>(const TaskID&) const = default;
};

struct SectionID {
    uint64_t value {};
    // An ID == 0 means the task has NO section
    static SectionID none() {return SectionID{0};}

    bool is_none() const {return value == 0;}

    explicit SectionID(uint64_t v) : value(v) {}

    auto operator<=>(const SectionID&) const = default;
};
}