#pragma once
#include "ids.hpp"
#include "state_report.hpp"
#include <vector>
#include <string>
#include <algorithm>
namespace Domain 
{
class Section {
public:
    // ----------Interface-------------
    Section(std::string name, SectionID id);
    void add_task(TaskID task);
    void remove_task(TaskID task);
    State::SectionReport report_state() const;
    // BASIC QUERIES
    const std::vector<TaskID>& get_tasks() const {return tasks;};
    bool is_empty() const {return tasks.empty();}
    bool contains_task(TaskID id) const {
        return std::find(tasks.begin(), tasks.end(), id) != tasks.end();
    }
    private:
    std::string name;
    SectionID id;
    std::vector<TaskID> tasks;

};
} // namespace Domain
