#pragma once
#include <cstdint>
#include <compare>
#include <functional>
namespace Domain {
template <typename T>
concept ID = requires(T id, uint64_t value) {
    T{value}; // ID is a type that can be built from a uint64_t
    id.value;
};
struct TaskID 
{
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
struct IDHash { // I don't know much about how hashes work, so I'll just use the hash form uint64_t
    template<typename T>
    size_t operator()(const T& id) const noexcept {
        return std::hash<uint64_t>{}(id.value);
    }
};

template <ID id>
bool operator==(const id& id1, const id& id2) {
    return id1.value == id2.value;
}
}

