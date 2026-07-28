#include "task_manager.hpp"

namespace Domain 
{
TaskID TaskManager::create_task(
        std::string title,
        std::string description,
        Priority priority,
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
State::ManagerFullReport TaskManager::report_state() const {
    //una vista de todas las tareas:
    auto task_reports = 
        tasks
        | std::views::values
        | std::views::transform([](const Task& t) { // could've also used std::views::transform(&Task::report_state) this is, a pointer to the member method
            return t.report_state();
            }
    );
    auto section_reports =
        sections
        | std::views::values
        | std::views::transform([](const Section& s) {
            return s.report_state();
        }
    );
    auto task_reports_vec =
    std::vector<State::TaskReport>(task_reports.begin(), task_reports.end());

    auto section_reports_vec =
        std::vector<State::SectionReport>(section_reports.begin(), section_reports.end());
    
    return {
        std::move(task_reports_vec),
        std::move(section_reports_vec),
        task_ids.report_state(),
        section_ids.report_state()
    };

}
} // namespace Domain