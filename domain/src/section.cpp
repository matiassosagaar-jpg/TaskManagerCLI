#include "section.hpp"

namespace Domain
{
Section::Section(std::string name, SectionID id) :
    name{std::move(name)}, id{id} {}

void Section::add_task(TaskID task) {
    tasks.emplace_back(task);
}
void Section::remove_task(TaskID task) {
    tasks.erase(std::remove(tasks.begin(),tasks.end(), task), tasks.end());
}

State::SectionReport Section::report_state() const {
    return {
        name,
        id,
        tasks
    };
}
} // namespace Domain