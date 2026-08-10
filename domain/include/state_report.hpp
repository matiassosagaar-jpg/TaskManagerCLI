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

struct SectionReport {
    std::string name;
    SectionID id;
    std::vector<TaskID> tasks;
};

struct IDGeneratorReport {
    uint64_t last_used;
};

struct ManagerFullReport {
    std::vector<TaskReport> task_reports;
    std::vector<SectionReport> section_reports;
    IDGeneratorReport last_task_id;
    IDGeneratorReport last_section_id;
};
} // namespace Domain
