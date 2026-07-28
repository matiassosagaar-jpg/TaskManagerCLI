#pragma once
#include <cstdint>
#include "ids.hpp"
#include "section.hpp"
#include "task.hpp"
#include "state_report.hpp"
#include "id_generator.hpp"
#include <unordered_map>
#include <algorithm>
#include <ranges>
namespace Domain 
{
class TaskManager {
public:
    TaskManager() = default;
    // INSERTION/DELETION OF TASKS/SECTIONS
    TaskID create_task(
        std::string title,
        std::string description,
        Priority priority,
        OptionalDate deadline,
        SectionID section
    );
    void remove_task(TaskID id);
    SectionID create_section(std::string name);
    void remove_section(SectionID id);
    void remove_section_and_tasks(SectionID id);
    // BASIC QUERIES
    Task& get_task(TaskID id) {return tasks.at(id);}
    const Task& get_task(TaskID id) const {return tasks.at(id);}
    Section& get_section(SectionID id) {return sections.at(id);}
    State::ManagerFullReport report_state() const;
    //------------Interface--------------
private:
    // ID GENERATORS
    IDGenerator<TaskID> task_ids;
    IDGenerator<SectionID> section_ids;
    // DATA OWNED
    std::unordered_map<SectionID, Section, IDHash> sections;
    std::unordered_map<TaskID, Task, IDHash> tasks;
};
} // namespace Domain