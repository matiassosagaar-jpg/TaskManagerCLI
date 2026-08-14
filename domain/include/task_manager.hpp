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
    void load_state(State::ManagerFullReport report);
    //------------Interface--------------
private:
    // ID GENERATORS
    IDGenerator<TaskID> task_ids;
    IDGenerator<SectionID> section_ids;
    // DATA OWNED
    std::unordered_map<SectionID, Section, IDHash> sections;
    std::unordered_map<TaskID, Task, IDHash> tasks;

    template<ID id, typename R, typename out>
    requires requires(const R& r) {
        requires std::same_as<
        std::remove_cvref_t<decltype(r.id)>,id>;
        { out::from_report(r) } -> std::same_as<out>;
    }
    static std::unordered_map<id, out, IDHash> build_data_from_report(
        const std::vector<R>& reports_vec
    ) {
        std::unordered_map<id, out, IDHash> output_map;
        for (const auto& rep : reports_vec) {
            auto item = out::from_report(rep);
            output_map.emplace(rep.id, item);
        }

        return output_map;
    }
};
} // namespace Domain