#include "persistence_helpers.hpp"

namespace Persistence
{
using namespace Domain;
using json = nlohmann::json;
std::string_view to_str(Priority priority) {
    auto lookup = std::ranges::find(
        priority_map,
        priority,
        &std::pair<Priority, std::string_view>::first
    );
    if (lookup == priority_map.end())
    throw std::invalid_argument("Unknown priority");
    return lookup->second;
}

Priority to_prio(const std::string_view& priority) {
    auto lookup = std::ranges::find(
        priority_map,
        priority,
        &std::pair<Priority, std::string_view>::second
    );
    if (lookup == priority_map.end())
    throw std::invalid_argument("Unknown priority");
    return lookup->first;
}


std::vector<std::string> get_priority_names() {
    std::vector<std::string> result;
    std::ranges::transform(
        array_all_priorities,
        std::back_inserter(result),
        [] (Priority p) {
            return std::string(to_str(p));
        }
    );
    return result;
}


} // namespace Persistence
