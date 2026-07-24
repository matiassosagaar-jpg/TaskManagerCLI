#include "task_manager.hpp"

namespace Domain 
{
TaskID TaskManager::create_task(
        std::string title,
        std::string description,
        Task::Priority priority,
        OptionalDate deadline,
        SectionID section
    ) {
        if (sections.find(section) == sections.end()) // throwing if the section doesn't exist
            throw std::invalid_argument("Section does not exist.");
        // CREATING TASK
        TaskID id {task_ids.next()}; // will advance the generator by 1 and return the current id
        tasks.emplace(
            id,
            Task(id, title, description,
                priority, deadline, section)
        );
        // ADDING TO SECTION
        sections.at(section).add_task(id); // don't want to implicitly create sections
        return id;
    }
void TaskManager::remove_task(TaskID id) {
    Task& task = get_task(id);
    // First we'll remove it's id from it's section
    sections.at(task.get_sectionID()).remove_task(id);
    // Now that we no longer need the sectionID from the task, we'll delete it
    tasks.erase(id);
}
SectionID TaskManager::create_section(std::string name) {
    // CREATING SECTION
    SectionID id {section_ids.next()};
    sections.emplace(
        id,
        Section(name, id)
    );
    return id;
}
void TaskManager::remove_section(SectionID id) {
    Section& section = get_section(id);
    if (!section.is_empty())
        throw std::invalid_argument("Cannot remove a non-empty section");
    sections.erase(id);
}
void TaskManager::remove_section_and_tasks(SectionID id) {
    // Removing all the tasks from the section
    Section& section = get_section(id);
    auto task_ids = section.get_tasks();
    for (auto task_id : task_ids)
        remove_task(task_id);
    // Now that we no longer need the section data
    sections.erase(id);
}
} // namespace Domain