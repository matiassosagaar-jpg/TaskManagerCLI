#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "ids.hpp"
#include "types.hpp"
namespace Domain::State

{
struct TaskReport {
    TaskID id;
    std::string title;
    std::string description;
    Priority priority;
    OptionalDate deadline;
    SectionID section;
};

inline bool operator==(const TaskReport& r1, const TaskReport& r2) {
    return r1.id == r2.id
        && r1.title == r2.title
        && r1.description == r2.description
        && r1.priority == r2.priority
        && r1.deadline == r2.deadline
        && r1.section == r2.section; 
}
struct SectionReport {
    std::string name;
    SectionID id;
    std::vector<TaskID> tasks;
};

inline bool operator==(const SectionReport& r1, const SectionReport& r2) {
    return r1.id == r2.id
        && r1.name == r2.name
        && r1.tasks == r2.tasks;
}

struct IDGeneratorReport {
    uint64_t last_used;
};

inline bool operator==(const IDGeneratorReport& r1, const IDGeneratorReport& r2) {
    return r1.last_used == r2.last_used;
}
struct ManagerFullReport {
    std::vector<TaskReport> task_reports;
    std::vector<SectionReport> section_reports;
    IDGeneratorReport last_task_id;
    IDGeneratorReport last_section_id;
};

inline bool operator==(const ManagerFullReport& r1, const ManagerFullReport& r2) {
        return r1.task_reports == r2.task_reports
            && r1.section_reports == r2.section_reports
            && r1.last_task_id == r2.last_task_id
            && r1.last_section_id == r2.last_section_id;
    }
} // namespace Domain
