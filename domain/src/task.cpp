#include "task.hpp"

namespace Domain 
{
Task::Task(
        TaskID id,
        std::string title,
        std::string description,
        Priority priority,
        OptionalDate deadline,
        SectionID section
    ) : id{id}, title{std::move(title)}, description{std::move(description)},
        priority{priority}, deadline {std::move(deadline)}, section{section} {
            if (!valid_deadline(deadline))
                throw std::invalid_argument("Invalid deadline");
        }

Task Task::from_report(const State::TaskReport& report) {
    return {
        report.id,
        report.title,
        report.description,
        report.priority,
        report.deadline,
        report.section
    };
}
State::TaskReport Task::report_state() const {
    return {
        id,
        title,
        description,
        priority,
        deadline,
        section
    };
}
bool Task::valid_deadline(const OptionalDate& deadline) {
    return !deadline || deadline->ok();
    // its valid if it does not exist or if it's a valid date
}


} // namespace Domain